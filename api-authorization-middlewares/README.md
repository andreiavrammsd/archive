# API Authorization Middlewares

Using middlewares to handle API authorization based on [Echo framework](https://echo.labstack.com/)
* Basic authorization
* Permissions (ACL)
* Account type

## Get

Clone this repository.

## Run

go run $(find . -type f -name "*.go" -not -name "*_test.go")

## Call

Login
```
curl -X POST \
     http://localhost:8118/login \
     -H 'Content-Type: application/json' \
     -d '{
   	"username": "admin",
   	"password": "admin"
   }'

{"token":"jwt.token"}
```

Users list
```
curl -X GET \
  http://localhost:8118/users \
  -H 'Authorization: Bearer jwt.token' \
  -H 'Content-Type: application/json'
```

Create user
```
curl -X POST \
  http://localhost:8118/users \
  -H 'Authorization: Bearer jwt.token' \
  -H 'Content-Type: application/json' \
  -d '{
	"username": "newuser",
	"password": "newuserpassword",
	"type": 2
}'
```

Delete user
```
curl -X DELETE \
  http://localhost:8118/users/3 \
  -H 'Authorization: Bearer jwt.token' \
  -H 'Content-type: application/json'
```

## Test

go test
