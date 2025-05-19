mod cli;
mod task;
mod tasks;

mod logger;
#[cfg(test)]
mod test_logger;

use clap::Parser;
use cli::{Cli, handle_command};
use logger::init_logger;
use std::process::ExitCode;

fn main() -> std::process::ExitCode {
    init_logger();

    let app = Cli::parse();

    match handle_command(app) {
        Ok(()) => ExitCode::from(0),
        Err(err) => {
            let msg = err.to_string();
            if !msg.is_empty() {
                eprintln!("{}", msg);
            }
            ExitCode::from(1)
        },
    }
}
