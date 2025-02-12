#![allow(dead_code)]

pub struct SbiRet {
    pub error: i64,
    pub value: i64,
}

#[inline(always)]
pub fn sbi_call(arg0: i64, arg1: i64, arg2: i64, arg3: i64, arg4: i64,
                arg5: i64, fid: i64, eid: i64) -> SbiRet {
    let mut error: i64;
    let mut value: i64;
    unsafe {
        core::arch::asm!(
            "ecall",
            inlateout("a0") arg0 => error,
            inlateout("a1") arg1 => value,
            in("a2") arg2,
            in("a3") arg3,
            in("a4") arg4,
            in("a5") arg5,
            in("a6") fid,
            in("a7") eid,
            options(nostack)
        );
    }
    SbiRet { error, value }
}

pub fn putchar(ch: char) {
    sbi_call(ch as i64, 0, 0, 0, 0, 0, 0, 1); // Console Putchar
}

pub fn console_puts(s: &str) {
    for ch in s.chars() {
        putchar(ch);
    }
}

pub fn wfi() {
    unsafe {
        core::arch::asm!("wfi", options(nomem, nostack));
    }
}