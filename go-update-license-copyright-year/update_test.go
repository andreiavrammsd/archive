package main

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

var copyrightPattern = "Copyright \\(c\\) ([0-9]{4})-?([0-9]{4})?"

func TestUpdateCopyrightYear(t *testing.T) {
	content := `Copyright (c) 2015-2016 Andrei Avram
		Permission is hereby granted, free of charge...`
	newYear := "2017"

	newContent, err := updateCopyrightYear(content, copyrightPattern, newYear)
	expected := `Copyright (c) 2015-2017 Andrei Avram
		Permission is hereby granted, free of charge...`

	assert.Equal(t, expected, newContent)
	assert.Equal(t, nil, err)
}

func TestUpdateCopyrightYearOneYear(t *testing.T) {
	content := `Copyright (c) 1998 Andrei Avram
		Permission is hereby granted, free of charge...`
	newYear := "2017"

	newContent, err := updateCopyrightYear(content, copyrightPattern, newYear)
	expected := `Copyright (c) 1998-2017 Andrei Avram
		Permission is hereby granted, free of charge...`

	assert.Equal(t, expected, newContent)
	assert.Equal(t, nil, err)
}

func TestUpdateCopyrightYeaHasNotChanged(t *testing.T) {
	content := `Copyright (c) 2017 Andrei Avram
		Permission is hereby granted, free of charge...`
	newYear := "2017"

	newContent, err := updateCopyrightYear(content, copyrightPattern, newYear)
	expected := ``

	assert.Equal(t, expected, newContent)
	assert.NotEqual(t, nil, err)
}
