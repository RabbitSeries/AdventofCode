use std::fs::OpenOptions;
use std::io::{BufWriter, Write};
use std::path::{self, Path, PathBuf};

fn scan_solutions(p: PathBuf) {
    println!("{:?}", p.display());
    let file = OpenOptions::new()
        .create(true) // create if it doesn't exist
        .append(true) // write at the end instead of truncating
        .open(p)
        .unwrap();
    let mut writer = BufWriter::new(file);
    match writer.write(b"[[bin]]\nname = 'day01'\npath = \"src/2025/day01/SecretEntrance.rs\"") {
        Ok(s) => println!("{} bytes written", s),
        Err(_e) => panic!(),
    }
}
fn main() {
    let xtask_dir = Path::new(env!("CARGO_MANIFEST_DIR"));
    let output_file = xtask_dir
        .parent()
        .unwrap()
        .join("aocrust")
        .join("Cargo.toml");
    match path::absolute(output_file.to_str().unwrap()) {
        Ok(abs_path) => scan_solutions(abs_path),
        Err(_e) => panic!(),
    }
}
