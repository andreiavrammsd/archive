<?php

use Phalcon\Cli\Task;

class QuitTask extends Task
{
    public function mainAction()
    {
        $this->connection->close();
        
        return ['message' => 'Goodbye!'];
    }
}
