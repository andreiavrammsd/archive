package taskrunner

import "sync"

// Task is the interface a task must implement.
type Task interface {
	// Run executes the work.
	Run() error
}

// AsyncTask is the interface a task should implement be to executed async.
type AsyncTask interface {
	Task
	// Concurrency returns the number of routines the task should be executed on.
	Concurrency() uint
}

// ErrorReporter is the function called on error.
type ErrorReporter func(error)

// Runner represents a task runner setup.
type Runner struct {
	tasks []Task
	er    ErrorReporter
	wg    sync.WaitGroup
}

// NewRunner creates a task runner.
func NewRunner(tasks []Task, er ErrorReporter) *Runner {
	return &Runner{tasks: tasks, er: er}
}

func (r *Runner) Run() error {
	for i := 0; i < len(r.tasks); i++ {
		switch t := r.tasks[i].(type) {
		case AsyncTask:
			for i := uint(0); i < t.Concurrency(); i++ {
				r.wg.Add(1)

				go func() {
					defer r.wg.Done()
					if err := t.Run(); err != nil {
						if r.er != nil {
							r.er(err)
						}
					}
				}()
			}
		case Task:
			r.wg.Add(1)

			if err := t.Run(); err != nil {
				r.wg.Done()
				if r.er != nil {
					r.er(err)
					return err
				}
			}
		}
	}

	return nil
}

func (r *Runner) Stop() {
}

func (r *Runner) Wait() {
}
