<?php

namespace Zonga\Tests\App\Response;

use PHPUnit\Framework\TestCase;
use Zonga\App\Response\Response;

class ResponseTest extends TestCase
{
    private $serializer;

    protected function setUp()
    {
        $this->serializer = $this->getMockBuilder('Zonga\App\Serializer\SerializerInterface')->getMock();
    }

    public function testGetContent()
    {
        $responseData = json_encode(
            [
                'event' => 'eventroute',
                'payload' => ['message' => true],
            ]
        );
        $this->serializer->method('serialize')->willReturn($responseData);
        $response = new Response($this->serializer);

        $this->assertEquals($responseData, $response->getContent());
    }
}
