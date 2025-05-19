use log::{Level, LevelFilter, Metadata, Record};

pub fn init_logger() {
    log::set_logger(&LOGGER).map(|()| log::set_max_level(LevelFilter::Info)).unwrap()
}

struct Logger;

impl log::Log for Logger {
    fn enabled(&self, metadata: &Metadata<'_>) -> bool {
        metadata.level() <= Level::Info
    }

    fn log(&self, record: &Record<'_>) {
        if self.enabled(record.metadata()) {
            match record.level() {
                Level::Info => println!("{}", record.args()),
                _ => eprintln!("{}", record.args()),
            }
        }
    }

    fn flush(&self) {}
}

static LOGGER: Logger = Logger;
