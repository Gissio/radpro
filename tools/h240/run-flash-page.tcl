# H240 application-page host transaction. License: MIT

proc h240_page_require_identity {} {
    set dpidr [string trim [capture {h240.dap dpreg 0}]]
    set cpuid [h240_word 0xe000ed00]
    if {$dpidr ne "0x0bb11477" || $cpuid != 0x410cc200} {
        error "Unexpected H240 debug identity"
    }
    set vectors [h240.mem read_memory 0x08000000 32 2]
    if {$vectors ne [h240.mem read_memory 0 32 2]} {
        error "Expected application flash at the boot alias"
    }
}

proc h240_page_require_idle {{expected -1}} {
    h240_require_converter_off
    set sr [h240_word 0x4002040c]
    set cr [h240_word 0x40020410]
    if {$sr != 0 || ($cr != 0x1800 && $cr != 0x1900 && $cr != 0x1c00 && $cr != 0x1d00) ||
        ($expected != -1 && $cr != $expected)} {
        error [format {Unexpected FCI status 0x%08x, control 0x%08x} $sr $cr]
    }
    if {[h240_word 0x40021014] & 1} {
        for {set channel 0} {$channel < 5} {incr channel} {
            lassign [h240.mem read_memory [expr {0x40020008 + $channel * 0x14}] 32 2] ccr count
            if {($ccr & 1) && (($ccr & 0x20) || $count)} {error "Active DMA channel $channel"}
        }
    }
    return $cr
}

proc h240_page_require_program_clock {} {
    set rcc_cr [h240_word 0x40021000]
    set rcc_cfgr [h240_word 0x40021004]
    set apb1 [h240_word 0x4002101c]
    set analog [h240_word 0x40021450]
    if {$rcc_cr != 0x03008083 || $rcc_cfgr != 0x0018000a ||
        ($apb1 != 0x10000002 && $apb1 != 0x10000012) || $analog != 6} {
        error [format {Expected 64 MHz programming profile (RCC_CR 0x%08x, CFGR 0x%08x, APB1ENR 0x%08x, analog 0x%08x)} \
            $rcc_cr $rcc_cfgr $apb1 $analog]
    }
}

proc h240_page_wait_clock {address expected phase} {
    for {set i 0} {$i < 50} {incr i} {
        h240_require_core 1
        if {[h240_word $address] == $expected} {return}
        sleep 10
    }
    error "Timed out waiting for $phase"
}

proc h240_page_prepare_clock {control} {
    h240_require_core 1
    h240_page_require_idle $control
    if {!($control & 0x100)} {
        if {[h240_word 0x40021000] != 0x00008083 || [h240_word 0x40021004] != 0 ||
            [h240_word 0x4002101c] != 2 || [h240_word 0x40021450] != 0 ||
            ([h240_word 0x40021014] & 1) || ([h240_word 0x40000400] & 1) ||
            ([h240_word 0x40013008] & 0x82) != 2} {
            error "Expected the 16 MHz standby profile with DMA gated, TIM3 stopped and SPI1 idle"
        }
        # The flash controller requires the factory analog and wait-state settings before programming.
        h240_require_core 1
        h240_write_word 0x4002101c 0x10000002
        h240_write_word 0x40021450 6
        if {[h240_word 0x4002101c] != 0x10000002 || [h240_word 0x40021450] != 6} {
            error "Factory analog timing did not take effect"
        }
        h240_require_core 1
        set control [expr {$control | 0x100}]
        h240_write_word 0x40020410 $control
        h240_page_require_idle $control
        h240_require_core 1
        h240_write_word 0x40021004 0x00180000
        if {[h240_word 0x40021004] != 0x00180000} {error "PLL configuration did not take effect"}
        h240_write_word 0x40021000 0x01008083
        h240_page_wait_clock 0x40021000 0x03008083 "PLL lock"
        h240_write_word 0x40021004 0x00180002
        h240_page_wait_clock 0x40021004 0x0018000a "PLL clock switch"
        h240_page_require_idle $control
        echo "Prepared 64 MHz programming clock from 16 MHz standby"
    }
    h240_page_require_program_clock
    return $control
}

proc h240_page_readback {address words} {
    set actual [h240.mem read_memory $address 32 [llength $words]]
    if {[lmap word $actual {format %08x $word}] ne [lmap word $words {format %08x $word}]} {
        error [format {Readback differs at 0x%08x} $address]
    }
}

proc h240_page_stage {address words} {
    # Only the idle SRAM loader runs while the host stages data and metadata.
    for {set offset 0} {$offset < [llength $words]} {incr offset 64} {
        set block [lrange $words $offset [expr {$offset + 63}]]
        set location [expr {$address + 4 * $offset}]
        h240_require_core 0
        h240.mem write_memory $location 32 $block
        h240_require_core 0
        h240_page_readback $location $block
    }
}

proc h240_page_wait {mailbox wanted} {
    set read_failures 0
    for {set i 0} {$i < 750} {incr i} {
        h240_require_core 0
        if {[catch {h240.mem read_memory $mailbox 32 12} fields]} {
            # Retry transient status reads during an outstanding page write.
            if {$wanted != 6 || [incr read_failures] == 5} {error $fields}
            sleep 20
            continue
        }
        set read_failures 0
        set status [lindex $fields 7]
        if {$status & 0x80000000} {
            error [format {Loader failed at sequence %u page 0x%08x, status 0x%08x detail 0x%08x control 0x%08x} \
                [lindex $fields 3] [lindex $fields 4] $status [lindex $fields 8] [lindex $fields 11]]
        }
        if {$status == $wanted} {return $fields}
        sleep 20
    }
    error "Timed out waiting for loader status $wanted"
}

proc h240_page_check_mailbox {fields challenge sequence page old_crc new_crc status expected_cr} {
    lassign $fields nonce response command seq address expected_old expected_new state detail before after cr
    if {$nonce != $challenge || $response != ($challenge ^ 0xffffffff) || $command != 0 ||
        $seq != $sequence || $address != $page || $expected_old != $old_crc || $expected_new != $new_crc ||
        $state != $status || $detail != 0 || $before != $old_crc || $after != $new_crc || $cr != $expected_cr} {
        error "Unexpected flash-loader mailbox for page $page, sequence $sequence: $fields"
    }
}

proc h240_page_require_recovery {entry mailbox loader} {
    h240_require_core 0
    h240_page_readback $entry $loader
    lassign [h240.mem read_memory $mailbox 32 12] nonce response command seq address old new state
    if {$nonce == 0 || $nonce == 0xffffffff || $response != ($nonce ^ 0xffffffff) || $command != 0 ||
        ($state != 1 && $state != 3 && $state != 6 && ($state < 0x80000001 || $state > 0x80000013))} {
        error "Recovery requires an idle or terminal instance of the reviewed SRAM loader"
    }
}

proc h240_page_trial {} {
    global env h240_page_challenge h240_page_loader h240_page_plan h240_page_recovery
    set directory $env(H240_FLASH_TRIAL_DIR)
    set entry 0x20000000
    set mailbox 0x20000630
    set stack 0x20000860
    set staging 0x20001000
    set pages {0x8001000 0x8002000 0x8003000 0x8004000 0x8005000 0x8006000 0x8007000 0x8008000 0x8009000 0x8000000}
    if {[llength $h240_page_plan] != [llength $pages] ||
        $entry + 4 * [llength $h240_page_loader] != $mailbox || $mailbox + 48 > $stack ||
        $stack > $staging || $staging + 4096 > 0x20002000} {error "Unexpected transaction layout"}
    foreach item $h240_page_plan page $pages {
        if {[llength $item] != 4 || [lindex $item 0] != $page || [llength [lindex $item 3]] != 1024} {
            error "Unexpected page plan"
        }
    }

    h240_page_require_identity
    set initial_control [h240_page_require_idle]
    targets h240.mem
    dump_image [file join $directory before.bin] 0x08000000 0x20000
    exec cmp -- [file join $directory expected-before.bin] [file join $directory before.bin]
    echo "Full flash matches the exact expected preimage"

    # Clear historical reset indication before touching the live core.
    h240_word 0xe000edf0
    h240_page_require_idle $initial_control
    if {$h240_page_recovery} {h240_page_require_recovery $entry $mailbox $h240_page_loader}
    h240_halt 0
    sleep 20
    h240_require_core 1
    h240_page_require_identity
    h240_page_require_idle $initial_control
    if {([h240_read_register 0x10] & 0x010001ff) != 0x01000000} {error "Expected Thumb Thread mode"}
    if {$h240_page_recovery} {
        set pc [format %08x [h240_read_register 0x0f]]
        if {$pc ni {20000010 20000014 20000016 20000018 2000001a}} {
            error "Recovery requires execution stopped in the SRAM loader wait loop"
        }
    }
    h240.mem read_memory $entry 32 [expr {($stack - $entry) / 4}]
    h240_set_register_checked 0x14 1
    h240_set_register_checked 0x11 $stack
    h240_set_register_checked 0x10 0x01000000
    h240_set_register_checked 0x0f $entry
    h240_load_checked $entry $h240_page_loader
    h240_load_checked $mailbox [list $h240_page_challenge 0 0 0 0 0 0 0 0 0 0 0]
    set current_control [h240_page_prepare_clock $initial_control]
    set locked_control [expr {$current_control | 0x400}]
    set dhcsr [h240_require_core 1]
    h240_write_word 0xe000edf0 [expr {0xa05f0001 | ($dhcsr & 8)}]

    set sequence 0
    set previous {0 0 0 1 0}
    foreach item $h240_page_plan {
        h240_page_check_mailbox [h240_page_wait $mailbox [lindex $previous 3]] $h240_page_challenge $sequence {*}$previous
        h240_page_require_idle $current_control
        incr sequence
        lassign $item page old_crc new_crc words
        h240_page_stage $staging $words
        h240_page_stage [expr {$mailbox + 12}] [list $sequence $page $old_crc $new_crc]
        h240_require_core 0
        h240_write_word [expr {$mailbox + 8}] [expr {$h240_page_challenge ^ $sequence ^ 0xa4b86310}]
        h240_page_check_mailbox [h240_page_wait $mailbox 3] $h240_page_challenge $sequence $page $old_crc $new_crc 3 $current_control
        h240_page_require_idle $current_control
        h240_page_require_program_clock
        h240_require_core 0
        echo [format {Committing page %u of 10 at 0x%08x} $sequence $page]
        h240_write_word [expr {$mailbox + 8}] [expr {$h240_page_challenge ^ $sequence ^ 0x731ef2c0}]
        sleep 250
        h240_page_check_mailbox [h240_page_wait $mailbox 6] $h240_page_challenge $sequence $page $old_crc $new_crc 6 $locked_control
        h240_page_require_idle $locked_control
        h240_page_readback $page $words
        h240_require_core 0
        set previous [list $page $old_crc $new_crc 6 $locked_control]
        set current_control $locked_control
    }

    h240_page_require_idle $locked_control
    h240_require_core 0
    dump_image [file join $directory after.bin] 0x08000000 0x20000
    exec cmp -- [file join $directory expected-after.bin] [file join $directory after.bin]
    h240_require_core 0
    echo "Full 128 KiB match the expected image. Core remains running in SRAM."
}

proc h240_page_main {} {
    if {[catch {h240_page_trial} failure]} {
        echo "Flash transaction stopped: $failure"
        echo "Leave the target powered. Check its live FCI state before considering halt or reset."
        catch {shutdown error}
    } else {
        shutdown
    }
}
