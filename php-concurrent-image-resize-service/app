#!/usr/local/bin/php
<?php

declare(strict_types=1);

$http = new swoole_http_server('0.0.0.0', 80);

$http->on('start', function ($server) {
    echo 'Server is started\n';
});

$log = fopen('process.log', 'a+');

$http->on('request', function ($request, $response) use ($log) {
    $pid = swoole_async::exec('sleep 3 && free -k | grep Mem | awk \'{print $3}\'', function ($result, $status) {
        echo sprintf('Memory used: %dk\n', $result);
    });
    echo sprintf('Running external process with PID %d...\n', $pid);

    $result = [
        'error' => '',
    ];

    $image = json_decode($request->rawcontent(), true);
    if (json_last_error() != JSON_ERROR_NONE) {
        $result['error'] = 'Invalid json';
        $response->status(400);
    } elseif (empty($image['src']) || empty($image['formats']) || !is_array($image['formats'])) {
        $result['error'] = 'Missing image content and/or conversion formats';
        $response->status(400);
    } else {
        go(function() use($log, $image) {
            $message = sprintf(
                'Process starting for url %s, in %d formats at %s\n',
                $image['src'],
                count($image['formats']),
                date('Y-m-d H:i:s')
            );
            
            $limit = 1024 * 1024 * 100;
            if (strlen($message) > $limit) {
                $message = sprintf(
                    '%s [truncated after %d chars]',
                    substr($message, 0, $limit),
                    $limit
                );
            }
            co::fwrite($log, $message);
        });

        $image['raw'] = file_get_contents($image['src']);

        $count = count($image['formats']);
        $chan = new chan($count);

        foreach ($image['formats'] as $format) {
            go(function () use($image, $format, $chan) {
                $format['q'] = $format['q'] ?? 100;

                $img = resize($image, $format);

                $result = [
                    'error' => $img['size'] == 0 ? 'Failed to convert' : '',
                    'path' => $img['path'],
                    'size' => $img['size'],
                    'width' => $format['w'],
                    'height' => $format['h'],
                    'quality' => (int)$format['q'],
                ];
                $chan->push($result);
            });
        }

        $result['images'] = [];
        for ($i = $count; $i > 0; $i--) {
            $result['images'] []= $chan->pop();
        }
    }

    $response->header('Content-Type', 'application/json');
    $response->end(json_encode($result));
});

$http->start();

function resize(array $image, array $format) : array {
    $thumb = imagecreatetruecolor($format['w'], $format['h']);
    $source = imagecreatefromstring($image['raw']);
    list($w, $h) = getimagesizefromstring($image['raw']);
    $success = imagecopyresized($thumb, $source, 0, 0, 0, 0, $format['w'], $format['h'], $w, $h);

    $path = '';
    $fileSize = 0;

    if ($success) {
        $path = getPath(md5($image['raw']), $format);

        if ($path != '') {
            $success = imagejpeg($thumb, $path, $format['q']);
            if ($success) {
                $fileSize = filesize($path);
            } else {
                $path = '';
            }
        }
    }

    return [
        'path' => $path,
        'size' => $fileSize,
    ];
}

function getPath(string $hash, array $size) : string {
    $path = '/images/' . date('YmdHi');
    if (!is_dir($path)) {
        $success = mkdir($path, 0644, true);
        if (!$success) {
            return '';
        }
    }

    return sprintf('%s/%s_%d_%d_%d.jpg', $path, $hash, $size['w'], $size['h'], $size['q']);
}
