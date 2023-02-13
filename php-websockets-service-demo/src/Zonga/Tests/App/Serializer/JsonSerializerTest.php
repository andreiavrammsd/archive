<?php

namespace Zonga\Tests\App\Serializer;

use PHPUnit\Framework\TestCase;
use Zonga\App\Serializer\JsonSerializer;

class JsonSerializerTest extends TestCase
{
    /**
     * @var JsonSerializer
     */
    private $serializer;

    protected function setUp()
    {
        $this->serializer = new JsonSerializer();
    }

    public function testSerialize()
    {
        $data = ['data' => 'to', 'serialize'];
        $this->assertEquals(json_encode($data), $this->serializer->serialize($data));
    }

    public function testUnserialize()
    {
        $data = json_encode(['data' => 'to', 'unserialize']);
        $this->assertEquals(json_decode($data, true), $this->serializer->unserialize($data));
    }
}
