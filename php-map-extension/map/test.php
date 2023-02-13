<?php

declare(strict_types=1);
error_reporting(E_ALL);

class PhpMap
{
    private $container = [];

    public function set(string $key, $value) {
        $this->container[$key] = $value;
    }

    public function get(string $key) {
        return $this->has($key) ? $this->container[$key] : null;
    }

    public function has(string $key) : bool {
        return array_key_exists($key, $this->container);
    }

    public function delete(string $key) {
        if ($this->has($key)) {
            unset($this->container[$key]);
        }
    }

    public function length() : int {
        return count($this->container);
    }

    public function range(callable $callback) : array {
        $items = [];
        foreach ($this->container as $key => $value) {
            if ($callback($value, $key) === true) {
                $items[$key] = $value;
            }
        }
        
        return $items;
    }
}

$phpMap = new PhpMap();
$map = new Map();

var_dump(run($phpMap) ==  run($map));

function run($map) : array{
    $mapClass = get_class($map);

    echo "\n\n";
    echo $mapClass;
    echo "\n\n";

    $map->set("a", 123);
    echo "a = ";
    var_dump($map->get("a"));
    echo "\n";

    $map->set("a", "b");
    echo "a = ";
    var_dump($map->get("a"));
    echo "\n";

    $map->set("b", false);
    echo "b = ";
    var_dump($map->get("b"));
    echo "\n";

    $map->set("c", null);
    echo "c = ";
    var_dump($map->get("c"));
    echo "\n";

    $map->set("defg", [0, null, false, true, "a"]);
    echo "defg = ";
    var_dump($map->get("defg"));
    echo "\n";

    $map->set("object", new $mapClass());
    echo "object = ";
    var_dump($map->get("object"));
    echo "\n";

    echo "get undefined key = ";
    var_dump($map->get("unset"));
    echo "\n";

    echo "length = ";
    var_dump($map->length());
    echo "\n";

    echo "delete undefined key = ";
    var_dump($map->delete("unset"));
    echo "\n";

    echo "delete key \"a\" = ";
    var_dump($map->delete("b"));
    echo "\n";

    echo "length = ";
    var_dump($map->length());
    echo "\n";

    echo "has defined key = ";
    var_dump($map->length());
    echo "\n";

    echo "has undefined key = ";
    var_dump($map->has("cc"));
    echo "\n";

    $items = $map->range(function($value, $key) {
        if (!is_object($value)) {
            return true;
        }
    });
    var_dump($items);

    return $items;
}
