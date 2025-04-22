package jobs

type Job interface {
	Run(payload interface{}) error
}

type Definition map[string]DefinitionEntry
type DefinitionEntry struct {
	Job     Job
	Payload interface{}
}
