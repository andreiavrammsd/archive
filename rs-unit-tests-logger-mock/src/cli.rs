use crate::{
    task::{Task, TaskResult},
    tasks,
};
use clap::{Parser, Subcommand};

#[derive(Parser)]
#[command(
    name = "rs-unit-tests-logger-mock",
    version = "v0.1.0",
    about = "How to mock a Rust logger for unit tests to debug logs written to stdout/stderr. Allows easier debugging."
)]
pub struct Cli {
    #[command(subcommand)]
    command: Commands,
}

#[derive(Subcommand)]
enum Commands {
    #[command(about = "Some task")]
    SomeTask(tasks::some_task::SomeTask),
}

pub fn handle_command(app: Cli) -> TaskResult {
    match app.command {
        Commands::SomeTask(task) => task.run(),
    }
}
