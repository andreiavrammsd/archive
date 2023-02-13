<?php

use Phalcon\Cli\Task;

class MainTask extends Task
{
    /**
     * @route hello
     * @param array $params
     * @return array
     */
    public function mainAction(array $params): array
    {
        return [
            'message' => sprintf('Hello, %s (%s)', $this->session->getUsername(), $params['name']),
            'routes' => $this->config->get('routes'),
        ];
    }

    /**
     * @route quit
     */
    public function quitAction()
    {
        $this->connection->close();
    }
}
