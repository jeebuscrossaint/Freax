use std::env;
use std::path::PathBuf;

fn main() {
    // Put the linker script somewhere the linker can find it
    let linker_script = PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap()).join("linker.ld");
    println!("cargo:rustc-link-arg=-T{}", linker_script.display());

    // Re-run the build script if the linker script is changed
    println!("cargo:rerun-if-changed=linker.ld");
}