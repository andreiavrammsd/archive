# Update Github repositories license copyright year

### I wanted to update the license files of all my Github repositories
 
#### What it does

Works with any license which has one of the following types of copyright lines:

* Copyright (c) 2014-2015 Andrei Avram
* Copyright (c) 2016 Andrei Avram

It will just update/add the current year (or any you wish) for the copyright line.
If the year is 2017, the above two lines will be changed to:

* Copyright (c) 2014-2017 Andrei Avram
* Copyright (c) 2016-2017 Andrei Avram

#### Usage
    
* go run !(*_test).go

* Possible actions
    * List repositories: Only the username is needed
    * Update license: Username and access token needed

* Required data will be prompted
    * Username: your Github username
    * Github access token: Your personal access token for Github API
    * License filenames: Possible names of your license files (leave empty to keep default)
    * Branch: The branch to make the changes on (leave empty to keep default)
    * Commit message: The commit message you wish to set for the changes (leave empty to keep default)
    * Current year (leave empty to keep current year)

* Github access token
    * https://github.com/settings/tokens
    * Select scopes: repo
