# GO Pipeline

## A very basic pipeline and workers example

Repeat the following request while changing ids and other properties, and watch the cli response.

```

curl -X POST \
    http://127.0.0.1:8080/ \
        -d '[
            {
                "id": 1,
                "first_name": "John",
                "last_name": "Doe"
            },
            {
                "id": 2,
                "first_name": "Johanna",
                "last_name": "Doe"
            },
            {
                "id": 3,
                "first_name": "Johanna Anne Marie",
                "last_name": "Doe Doe"
            }
    ]'
 ```
