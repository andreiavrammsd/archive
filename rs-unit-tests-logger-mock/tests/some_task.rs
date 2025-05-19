use assert_cmd::Command;
use predicates::str::contains;

#[test]
fn test_some_task() {
    let mut cmd = Command::cargo_bin(env!("CARGO_PKG_NAME")).unwrap();
    cmd.args(["some-task", "3"])
        .assert()
        .success()
        .stdout(contains("Some info message 0"))
        .stderr(contains("Some error message 1"))
        .stdout(contains("Some info message 2"));
}
