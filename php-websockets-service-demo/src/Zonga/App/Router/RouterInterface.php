<?php

namespace Zonga\App\Router;

interface RouterInterface
{
    /**
     * @param array $routes
     */
    public function __construct(array $routes);

    /**
     * @param string $event
     * @throws EventNotFoundException
     */
    public function setEvent(string $event);

    /**
     * @return string
     */
    public function getTask() : string;

    /**
     * @return string
     */
    public function getAction() : string;
}
