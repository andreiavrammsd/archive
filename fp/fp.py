# https://book.pythontips.com/en/latest/map_filter.html

from enum import Enum
from functools import reduce
from typing import NamedTuple, List, Callable


class Type(Enum):
    APPLE = 1
    BANANA = 2
    PEAR = 3

    def __str__(self):
        return self.name.lower()


class Color(Enum):
    GREEN = 1
    YELLOW = 2
    RED = 3

    def __str__(self):
        return self.name.lower()


Weight = int


class Fruit(NamedTuple):
    type: Type
    color: Color
    weight: Weight


class Filter(NamedTuple):
    types: List[Type]
    colors: List[Color]
    weight_more_than: Weight


def select_by_types(types: List[Type]):
    return lambda fruits: list(filter(lambda fruit: fruit.type in types, fruits))


def select_by_colors(colors: List[Color]):
    return lambda fruits: list(filter(lambda fruit: fruit.color in colors, fruits))


def select_if_weighs_more_than(weight: Weight):
    return lambda fruits: list(filter(lambda fruit: fruit.weight > weight, fruits))


def list_fruits(fruits: List[Fruit]):
    for fruit in fruits:
        print('- %s %s (%dg)' % (fruit.color, fruit.type, fruit.weight))
    return fruits


def get_total_weight(fruits: List[Fruit]):
    return sum(fruit.weight for fruit in fruits)


fruit_list = [
    Fruit(Type.APPLE, Color.GREEN, 30),
    Fruit(Type.APPLE, Color.GREEN, 130),
    Fruit(Type.APPLE, Color.YELLOW, 130),
    Fruit(Type.APPLE, Color.GREEN, 330),
    Fruit(Type.APPLE, Color.GREEN, 630),
    Fruit(Type.APPLE, Color.RED, 630),
    Fruit(Type.BANANA, Color.GREEN, 130),
    Fruit(Type.APPLE, Color.YELLOW, 131),
    Fruit(Type.PEAR, Color.GREEN, 30),
    Fruit(Type.PEAR, Color.GREEN, 180),
]

fruit_filter = Filter([Type.APPLE, Type.PEAR], [Color.GREEN, Color.RED], 130)

fruit_criteria = [
    select_by_types(fruit_filter.types),
    select_by_colors(fruit_filter.colors),
    select_if_weighs_more_than(fruit_filter.weight_more_than),
    list_fruits,
    get_total_weight,
]


def get_weight(fruits: List[Fruit], criteria: List[Callable]):
    return reduce((lambda input, criterion: criterion(input)), criteria, fruits)


assert get_weight(fruit_list, fruit_criteria) == 1770
assert len(fruit_list) == 10
