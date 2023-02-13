<?php

namespace Zonga\Tests\App\Request;

use PHPUnit\Framework\TestCase;
use Zonga\App\Request\Request;

class RequestTest extends TestCase
{
    private $serializer;

    protected function setUp()
    {
        $this->serializer = $this->getMockBuilder('Zonga\App\Serializer\SerializerInterface')->getMock();
    }

    public function testSetRequest()
    {
        $requestData = [
            'event' => 'eventroute',
            'payload' => ['message' => true],
        ];
        $this->serializer->method('unserialize')->willReturn($requestData);
        $request = new Request($this->serializer);
        $request->setRequest('{"a" : 1}');
        
        $this->assertEquals($requestData['event'], $request->getEvent());
        $this->assertEquals($requestData['payload'], $request->getPayload());
    }

    /**
     * @expectedException \Zonga\App\Request\BadRequestException
     * @expectedExceptionMessage Invalid request body
     */
    public function testValidateRequestInvalidRequestBody()
    {
        $this->serializer->method('unserialize')->willReturn('');
        $request = new Request($this->serializer);
        $request->setRequest('{"a" : 1}');
        $request->validateRequest();
    }

    /**
     * @expectedException \Zonga\App\Request\BadRequestException
     * @expectedExceptionMessage Missing event
     */
    public function testValidateRequestMissingEvent()
    {
        $this->serializer->method('unserialize')->willReturn(['event_' => false]);
        $request = new Request($this->serializer);
        $request->setRequest('{"a" : 1}');
        $request->validateRequest();
    }

    /**
     * @expectedException \Zonga\App\Request\BadRequestException
     * @expectedExceptionMessage Invalid payload
     * @dataProvider requestDataInvalidPayload
     */
    public function testValidateRequestInvalidPayload($requestData)
    {
        $this->serializer->method('unserialize')->willReturn($requestData);
        $request = new Request($this->serializer);
        $request->setRequest('{"a" : 1}');
        $request->validateRequest();
    }

    /**
     * @dataProvider requestDataEvent
     * @param array $request
     * @param string $expected
     */
    public function _testGetEvent($requestData, $expected)
    {
        $this->serializer->method('unserialize')->willReturn($requestData);
        $request = new Request($this->serializer);
        $request->setRequest('{"a" : 1}');

        $this->assertEquals($expected, $request->getEvent());
    }

    /**
     * @dataProvider requestDataPayload
     * @param array $requestData
     * @param mixed $expected
     */
    public function testGetPayload($requestData, $expected)
    {
        $this->serializer->method('unserialize')->willReturn($requestData);
        $request = new Request($this->serializer);
        $request->setRequest('{"a" : 1}');

        $this->assertEquals($expected, $request->getPayload());
    }

    /**
     * @return array
     */
    public function requestDataInvalidPayload()
    {
        return [
            [
                [
                    'payload' => false,
                    'event' => 'eventroute'
                ],
            ],
            [
                [
                    'payload' => null,
                    'event' => 'eventroute'
                ],
            ],
            [
                [
                    'payload' => '',
                    'event' => 'eventroute'
                ],
            ],
        ];
    }

    /**
     * @return array
     */
    public function requestDataEvent()
    {
        return [
            [
                ['event' => 'eventroute'],
                'eventroute',
            ],
        ];
    }

    /**
     * @return array
     */
    public function requestDataPayload()
    {
        return [
            [
                ['payload' => []],
                [],
            ],
            [
                ['payload' => ['a']],
                ['a'],
            ],
            [
                ['payload' => ['a' => 1]],
                ['a' => 1],
            ],
            [
                ['payload_missing' => ''],
                [],
            ],
        ];
    }
}
