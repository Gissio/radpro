# Run with OpenOCD noinit, the actual host transaction, and no adapter.
source [file join [file dirname [info script]] openocd-support.tcl]

rename targets builtin_targets
proc targets {name} {}
proc dump_image {path address length} {
    global scenario commits env
    if {$address != 0x08000000 || $length != 0x20000} {error "Unexpected dump bounds"}
    set filename [file tail $path]
    if {$filename eq "before.bin" && $scenario eq "wrong_preimage"} {
        set fd [open $path w]
        puts $fd wrong
        close $fd
        return
    }
    if {$filename eq "after.bin" && $commits != 10} {error "Premature flash dump"}
    file copy -force [file join $env(H240_FLASH_TRIAL_DIR) expected-$filename] $path
}

proc sleep {milliseconds} {}
proc h240_page_require_identity {} {}
proc h240_read_register {selector} {
    switch -- $selector {
        0x0f {return 0x20000016}
        0x10 {return 0x01000000}
        default {error "Unexpected debug register read $selector"}
    }
}
proc h240_set_register_checked {selector value} {
    global primask
    h240_require_core 1
    if {$selector == 0x14} {set primask $value}
}
proc h240_require_core {halted {debug_enabled 1}} {
    global is_halted
    if {$is_halted != $halted} {error "Unexpected core state"}
    return 0x30003
}
proc h240_halt {debug_enabled} {
    global is_halted halts
    incr halts
    set is_halted 1
}
proc h240_word {address} {
    global controller_status controller_control clock_registers
    if {[dict exists $clock_registers $address]} {return [dict get $clock_registers $address]}
    switch -- $address {
        0x4002040c {return $controller_status}
        0x40020410 {return [format 0x%08x $controller_control]}
        0x40021014 {return 0}
        0x4002203c {return 0x4302}
        0x400220c0 - 0x400220d0 {return 0}
        0xe000edf0 {return 0}
        default {error "Unexpected hardware read $address"}
    }
}
proc h240_load_checked {address words} {
    h240_require_core 1
    h240.mem write_memory $address 32 $words
}
proc h240.mem {action address width data} {
    global ram flash mock_mailbox mailbox_read_failures
    set address [expr {$address + 0}]
    if {$width != 32} {error "Unexpected width"}
    if {$action eq "write_memory"} {
        if {$address < 0x20000000 || $address + 4 * [llength $data] > 0x20002000} {error "Non-SRAM write"}
        foreach word $data {
            dict set ram $address $word
            incr address 4
        }
        return
    }
    if {$address < 0x20000000} {return [dict get $flash $address]}
    if {$address == $mock_mailbox && $data == 12 && $mailbox_read_failures} {
        incr mailbox_read_failures -1
        error "Simulated SWD read failure"
    }
    set words {}
    for {set i 0} {$i < $data} {incr i} {
        lappend words [expr {[dict exists $ram $address] ? [dict get $ram $address] : 0}]
        incr address 4
    }
    return $words
}

proc h240_write_word {address command} {
    global scenario is_halted commits controller_status controller_control flash h240_page_plan mock_mailbox mailbox_read_failures
    global clock_registers clock_writes primask h240_page_loader h240_page_challenge
    set mailbox $mock_mailbox
    if {$address in {0x4002101c 0x40021450 0x40020410 0x40021004 0x40021000}} {
        h240_require_core 1
        if {$primask != 1 || $commits != 0} {error "Clock write outside the masked SRAM setup"}
        h240_page_readback 0x20000000 $h240_page_loader
        h240_page_readback $mailbox [list $h240_page_challenge 0 0 0 0 0 0 0 0 0 0 0]
        lappend clock_writes [list $address [format 0x%08x $command]]
        switch -- $address {
            0x40020410 {set controller_control $command}
            0x40021000 {
                if {$scenario ne "pll_lock_timeout"} {set command [expr {$command | 0x02000000}]}
                dict set clock_registers $address $command
            }
            0x40021004 {
                if {$command == 0x00180002 && $scenario ne "pll_switch_timeout"} {set command 0x0018000a}
                dict set clock_registers $address $command
            }
            default {dict set clock_registers $address $command}
        }
        return
    }
    lassign [h240.mem read_memory $mailbox 32 12] nonce complement idle seq page old_crc new_crc status
    if {$address == 0xe000edf0 && $command == 0xa05f0001 && $is_halted} {
        set is_halted 0
        h240.mem write_memory [expr {$mailbox + 4}] 32 [list [expr {$nonce ^ 0xffffffff}]]
        h240.mem write_memory [expr {$mailbox + 28}] 32 {1}
        return
    }
    lassign [lindex $h240_page_plan $commits] approved_page approved_old approved_new input
    if {$address != $mailbox + 8 || $is_halted || $seq != $commits + 1 || $page != $approved_page ||
        $old_crc != $approved_old || $new_crc != $approved_new} {
        error "Unexpected command metadata: $address $is_halted $seq $page $old_crc $new_crc, plan $commits $approved_page $approved_old $approved_new"
    }
    h240_page_readback 0x20001000 $input
    if {$command == ($nonce ^ $seq ^ 0xa4b86310) && ($status == 1 || $status == 6)} {
        if {$scenario eq "rejected_prepare"} {incr old_crc}
        h240.mem write_memory [expr {$mailbox + 28}] 32 [list 3 0 $old_crc $new_crc $controller_control]
    } elseif {$command == ($nonce ^ $seq ^ 0x731ef2c0) && $status == 3} {
        if {[h240_word 0x40021000] != 0x03008083 || [h240_word 0x40021004] != 0x0018000a ||
            [h240_word 0x40021450] != 6 || !($controller_control & 0x100)} {
            error "Commit without the 64 MHz programming clock"
        }
        incr commits
        if {$commits == 1 && $scenario eq "transient_read"} {set mailbox_read_failures 2}
        if {$commits == 1 && $scenario eq "persistent_read"} {set mailbox_read_failures 10}
        if {$scenario eq "busy"} {
            set controller_status 1
            h240.mem write_memory [expr {$mailbox + 28}] 32 [list 0x8000000a 1 $old_crc $new_crc 0]
        } else {
            dict set flash [expr {$page + 0}] $input
            set controller_control [expr {$controller_control | 0x400}]
            h240.mem write_memory [expr {$mailbox + 28}] 32 [list 6 0 $old_crc $new_crc $controller_control]
            if {$scenario eq "wrong_relock"} {set controller_control 0x1c00}
        }
    } else {
        error "Unexpected command/state combination"
    }
}

set directory $env(H240_FLASH_TRIAL_DIR)
set file [open [file join $directory expected-before.bin] w]
puts $file before
close $file
set file [open [file join $directory expected-after.bin] w]
puts $file after
close $file
set h240_page_challenge 0x12345678
set mock_mailbox 0x20000630
set h240_page_loader [lrepeat 396 0xa5]
set h240_page_plan {}
foreach page {0x8001000 0x8002000 0x8003000 0x8004000 0x8005000 0x8006000 0x8007000 0x8008000 0x8009000 0x8000000} {
    lappend h240_page_plan [list $page [expr {$page + 1}] [expr {$page + 2}] [lrepeat 1024 [expr {$page}]]]
}
foreach {scenario profile expected_halts expected_commits expected_clock_length expected_error} {
    success pll 1 10 0 {}
    restore_hsi hsi 1 10 6 {}
    recovery_partial hsi 1 10 6 {}
    recovery_not_loader hsi 0 0 0 {Readback differs}
    wrong_preimage hsi 0 0 0 differ
    enabled_converter pll 0 0 0 {TIM14 clock gated}
    rejected_prepare pll 1 0 0 mailbox
    busy pll 1 1 0 {status 0x8000000a}
    transient_read pll 1 10 0 {}
    persistent_read pll 1 1 0 {Simulated SWD read failure}
    wrong_relock pll 1 1 0 {control 0x00001c00}
    bad_pll_analog pll 1 0 0 {64 MHz programming profile}
    bad_hsi_cfgr hsi 1 0 0 {16 MHz standby profile}
    bad_hsi_apb hsi 1 0 0 {16 MHz standby profile}
    bad_hsi_tim3 hsi 1 0 0 {16 MHz standby profile}
    bad_hsi_spi hsi 1 0 0 {16 MHz standby profile}
    bad_hsi_fci hsi 0 0 0 {FCI status 0x00000002}
    pll_lock_timeout hsi 1 0 5 {PLL lock}
    pll_switch_timeout hsi 1 0 6 {PLL clock switch}
} {
    set is_halted 0
    set halts 0
    set commits 0
    set mailbox_read_failures 0
    set controller_status 0
    set primask 0
    set clock_writes {}
    set clock_registers {0x40000400 0 0x40013008 2}
    if {$profile eq "hsi"} {
        set controller_control 0x1c00
        dict set clock_registers 0x40021000 0x00008083
        dict set clock_registers 0x40021004 0x00000000
        dict set clock_registers 0x4002101c 0x00000002
        dict set clock_registers 0x40021450 0x00000000
    } else {
        set controller_control 0x1900
        dict set clock_registers 0x40021000 0x03008083
        dict set clock_registers 0x40021004 0x0018000a
        dict set clock_registers 0x4002101c 0x10000012
        dict set clock_registers 0x40021450 0x00000006
    }
    switch -- $scenario {
        enabled_converter {dict set clock_registers 0x4002101c 0x10000112}
        bad_pll_analog {dict set clock_registers 0x40021450 0}
        bad_hsi_cfgr {dict set clock_registers 0x40021004 0x00000400}
        bad_hsi_apb {dict set clock_registers 0x4002101c 0x00000012}
        bad_hsi_tim3 {dict set clock_registers 0x40000400 1}
        bad_hsi_spi {dict set clock_registers 0x40013008 0x82}
        bad_hsi_fci {set controller_status 2}
    }
    set h240_page_recovery [expr {$scenario in {recovery_partial recovery_not_loader}}]
    set ram {}
    set flash {}
    if {$scenario eq "recovery_partial"} {
        h240.mem write_memory 0x20000000 32 $h240_page_loader
        h240.mem write_memory $mock_mailbox 32 {0x87654321 0x789abcde 0 1 0x08001000 0 0 0x8000000e 0 0 0 0x1800}
        set controller_control 0x1800
    }
    set wait_control [format 0x%08x [expr {$controller_control | 0x100}]]
    set sequence [list {0x4002101c 0x10000002} {0x40021450 0x00000006} [list 0x40020410 $wait_control] \
        {0x40021004 0x00180000} {0x40021000 0x01008083} {0x40021004 0x00180002}]
    set failed [catch {h240_page_trial} result]
    if {$halts != $expected_halts || $commits != $expected_commits ||
        $failed != ($expected_error ne "") || ($failed && [string first $expected_error $result] < 0)} {
        error "$scenario failed ($halts halt, $commits commits): $result"
    }
    if {$clock_writes ne [lrange $sequence 0 [expr {$expected_clock_length - 1}]]} {
        error "$scenario wrote an unexpected clock sequence: $clock_writes"
    }
}
echo "H240 application flash host tests passed"
