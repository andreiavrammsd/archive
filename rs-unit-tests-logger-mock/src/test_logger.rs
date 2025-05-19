use log::{Level, LevelFilter, Metadata, Record};
use std::cell::RefCell;
use std::sync::Once;

pub fn init_logger() {
    START.call_once(|| {
        log::set_logger(&LOGGER).map(|()| log::set_max_level(LevelFilter::Info)).unwrap()
    });
}

pub struct LogRecord {
    pub level: Level,
    pub args: String,
}

pub fn get_logs() -> Vec<LogRecord> {
    LOG_RECORDS.take()
}

static START: Once = Once::new();
static LOGGER: TestLogger = TestLogger;

thread_local! {
    static LOG_RECORDS: RefCell<Vec<LogRecord>> = const {RefCell::new(Vec::new())};

}

struct TestLogger;

impl log::Log for TestLogger {
    fn enabled(&self, metadata: &Metadata<'_>) -> bool {
        metadata.level() <= Level::Info
    }

    fn log(&self, record: &Record<'_>) {
        if self.enabled(record.metadata()) {
            LOG_RECORDS.with(|rec| {
                let mut records = rec.borrow_mut();
                records.push(LogRecord { level: record.level(), args: record.args().to_string() });
            });
        }
    }

    fn flush(&self) {}
}
