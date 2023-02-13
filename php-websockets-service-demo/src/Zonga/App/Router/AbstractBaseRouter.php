<?php

namespace Zonga\App\Router;

abstract class AbstractBaseRouter
{
    /**
     * @var array
     */
    private $routes;

    /**
     * @var string
     */
    private $task;

    /**
     * @var string
     */
    private $action;

    /**
     * @param array $routes
     */
    public function __construct(array $routes)
    {
        $this->routes = $routes;
    }

    /**
     * @param string $event
     * @throws EventNotFoundException
     */
    public function setEvent(string $event)
    {
        if (!isset($this->routes[$event])) {
            throw new EventNotFoundException(sprintf('Event not found: %s', $event));
        }

        list ($task, $action) = explode('::', $this->routes[$event]);

        $this->task = $task;
        $this->action = $action;
    }

    /**
     * @return string
     */
    public function getTask() : string
    {
        return $this->task;
    }

    /**
     * @return string
     */
    public function getAction() : string
    {
        return $this->action;
    }
}
