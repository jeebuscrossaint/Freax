#![no_std]
#![no_main]
#![feature(naked_functions)]

mod sbi;

use core::ptr;
use core::arch::asm;

type Uint32T = u32;
type SizeT = Uint32T;

unsafe extern "C" {
    static __bss: u8;
    static __bss_end: u8;
    static __stack_top: u8;
}

#[unsafe(no_mangle)]
pub extern "C" fn memset(buf: *mut u8, c: u8, n: SizeT) -> *mut u8 {
    let mut p = buf;
    for _ in 0..n {
        unsafe {
            ptr::write(p, c);
            p = p.add(1);
        }
    }
    buf
}

#[unsafe(no_mangle)]
pub extern "C" fn kernel_main() -> ! {
    use crate::sbi::console_puts;

    unsafe {
        memset(&__bss as *const _ as *mut u8, 0, &__bss_end as *const _ as SizeT - &__bss as *const _ as SizeT);
    }

    console_puts("\n\nHello World!\n");

    loop {
        sbi::wfi();
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn boot() -> ! {
    unsafe {
        asm!(
        "mv sp, {0}",
        "j kernel_main",
        in(reg) &__stack_top,
        options(noreturn)
        );
    }
}

#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {
    loop {}
}