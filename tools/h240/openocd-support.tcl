# H240 MEM-AP access, converter guard and Cortex-M0 control. License: MIT

proc h240_word {address} {
    return [lindex [h240.mem read_memory $address 32 1] 0]
}

proc h240_write_word {address value} {
    h240.mem write_memory $address 32 [list $value]
}

proc h240_require_converter_off {} {
    set pa15 [h240_word 0x4002203c]
    set gpio_out [h240_word 0x400220c0]
    set gpio_in [h240_word 0x400220d0]
    set apb1enr [h240_word 0x4002101c]
    echo [format {PA15 config 0x%08x, GPIOA output 0x%08x, input 0x%08x, RCC_APB1ENR 0x%08x} \
        $pa15 $gpio_out $gpio_in $apb1enr]
    if {($pa15 & 0xffff) != 0x4302 || (($gpio_out | $gpio_in) & 0x8000) ||
        ($apb1enr & 0x100)} {
        error "Expected PA15 driven low as GPIO and the TIM14 clock gated"
    }
}

proc h240_core_status {{debug_enabled 1}} {
    set dhcsr [h240_word 0xe000edf0]
    # Reading DHCSR clears the sticky reset bit, so reject it on every read here.
    if {($dhcsr & 0x02080000) || ($debug_enabled && !($dhcsr & 1)) ||
        (($dhcsr & 1) && ($dhcsr & 4))} {
        error [format {Unexpected core state or reset (DHCSR 0x%08x)} $dhcsr]
    }
    return $dhcsr
}

proc h240_require_core {halted {debug_enabled 1}} {
    set dhcsr [h240_core_status $debug_enabled]
    if {($dhcsr & 0x00020000) != ($halted << 17)} {
        error [format {Unexpected halt status (DHCSR 0x%08x)} $dhcsr]
    }
    return $dhcsr
}

proc h240_halt {debug_enabled} {
    set dhcsr [h240_require_core 0 $debug_enabled]
    # C_MASKINTS must be zero on debug enable and preserved once enabled.
    set maskints [expr {($dhcsr & 1) ? ($dhcsr & 8) : 0}]
    h240_write_word 0xe000edf0 [expr {0xa05f0003 | $maskints}]
    for {set i 0} {$i < 50} {incr i} {
        if {[h240_core_status] & 0x00020000} {return}
        sleep 1
    }
    error "Timed out waiting for Cortex-M0 halt"
}

proc h240_wait_register {} {
    for {set i 0} {$i < 50} {incr i} {
        if {[h240_require_core 1] & 0x00010000} {return}
        sleep 1
    }
    error "Timed out waiting for Cortex-M0 register transfer"
}

proc h240_read_register {selector} {
    h240_require_core 1
    h240_write_word 0xe000edf4 $selector
    h240_wait_register
    return [h240_word 0xe000edf8]
}

proc h240_set_register_checked {selector value} {
    h240_require_core 1
    h240_write_word 0xe000edf8 $value
    h240_write_word 0xe000edf4 [expr {0x10000 | $selector}]
    h240_wait_register
    set actual [h240_read_register $selector]
    if {$actual != $value} {
        error [format {Cortex-M0 register 0x%02x readback 0x%08x, expected 0x%08x} \
            $selector $actual $value]
    }
}

proc h240_load_checked {address image} {
    # Bound each write so a reset during loading cannot start more SRAM writes.
    for {set offset 0} {$offset < [llength $image]} {incr offset 64} {
        set block [lrange $image $offset [expr {$offset + 63}]]
        set block_address [expr {$address + 4 * $offset}]
        h240_require_core 1
        h240.mem write_memory $block_address 32 $block
        h240_require_core 1
        set loaded [h240.mem read_memory $block_address 32 [llength $block]]
        if {[lmap word $loaded {format %08x $word}] ne [lmap word $block {format %08x $word}]} {
            error [format {SRAM payload readback differs at 0x%08x} $block_address]
        }
    }
}
