const fruitList = [
    {type: 'apple', color: 'green', weight: 30,},
    {type: 'apple', color: 'green', weight: 130,},
    {type: 'apple', color: 'yellow', weight: 130,},
    {type: 'apple', color: 'green', weight: 330,},
    {type: 'apple', color: 'green', weight: 630,},
    {type: 'apple', color: 'red', weight: 630,},
    {type: 'banana', color: 'green', weight: 130,},
    {type: 'apple', color: 'yellow', weight: 131,},
    {type: 'pear', color: 'green', weight: 30,},
    {type: 'pear', color: 'green', weight: 180,},
];

const filters = {
    types: ['apple', 'pear'],
    colors: ['green', 'red',],
    weightMoreThan: 130,
};

const selectTypes = (types) => (fruits) => fruits.filter((fruit) => types.includes(fruit.type))
const selectColors = (colors) => (fruits) => fruits.filter((fruit) => colors.includes(fruit.color))
const weighsMoreThan = (weight) => (fruits) => fruits.filter((fruit) => fruit.weight > weight)
const totalWeight = (fruits) => fruits.reduce((total, fruit) => total + fruit.weight, 0)
const list = (fruits) => {
    console.log(fruits);
    return fruits;
}

const criteria = [
    selectTypes(filters.types),
    selectColors(filters.colors),
    weighsMoreThan(filters.weightMoreThan),
    list,
    totalWeight,
];

const getWeight = (fruits, criteria) => {
    return criteria.reduce((input, criterion) => criterion(input), fruits)
}

const weight = getWeight(fruitList, criteria)

const assert = require('assert');
assert(weight === 1770);

