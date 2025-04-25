package taskrunner

type Task interface {
	Run() error
}

type AsyncTask interface {
	Task
	Concurrency() uint
}

type ErrorReporter func(error)

type Runner struct {
	tasks []Task
	er    ErrorReporter
}

func NewRunner(tasks []Task, er ErrorReporter) *Runner {
	return &Runner{tasks: tasks, er: er}
}

func (r *Runner) Run() error {
	for i := 0; i < len(r.tasks); i++ {
		switch t := r.tasks[i].(type) {
		case AsyncTask:
			for i := uint(0); i < t.Concurrency(); i++ {
				go func() {
					if err := t.Run(); err != nil {
						if r.er != nil {
							r.er(err)
						}
					}
				}()
			}
		case Task:
			if err := t.Run(); err != nil {
				if r.er != nil {
					r.er(err)
				}
			}
		}
	}

	return nil
}
