use crate::task::{Task, TaskResult};
use clap::Args;
use log::{error, info};

#[derive(Args)]
pub struct SomeTask {
    pub message_count: usize,
}

impl Task for SomeTask {
    fn run(&self) -> TaskResult {
        (0..self.message_count).for_each(|i| match i % 2 {
            0 => info!("Some info message {}", i),
            _ => error!("Some error message {}", i),
        });

        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use log::Level;

    use super::*;
    use crate::test_logger::{get_logs, init_logger};

    #[test]
    fn test_some_task_with_no_message() {
        init_logger();

        let task = SomeTask { message_count: 0 };
        assert!(task.run().is_ok());

        let logs = get_logs();
        assert!(logs.is_empty());
    }

    #[test]
    fn test_some_task_with_one_message() {
        init_logger();

        let task = SomeTask { message_count: 1 };
        assert!(task.run().is_ok());

        let logs = get_logs();
        assert_eq!(logs.len(), 1);

        assert_eq!(logs[0].level, Level::Info);
        assert_eq!(logs[0].args, "Some info message 0");
    }

    #[test]
    fn test_some_task_with_multiple_messages() {
        init_logger();

        let task = SomeTask { message_count: 3 };
        assert!(task.run().is_ok());

        let logs = get_logs();
        assert_eq!(logs.len(), 3);

        assert_eq!(logs[0].level, Level::Info);
        assert_eq!(logs[0].args, "Some info message 0");

        assert_eq!(logs[1].level, Level::Error);
        assert_eq!(logs[1].args, "Some error message 1");

        assert_eq!(logs[2].level, Level::Info);
        assert_eq!(logs[2].args, "Some info message 2");
    }
}
