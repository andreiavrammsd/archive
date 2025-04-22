#include <cassert>

struct Object {
    int prop_1{};
    int prop_2{};
    int prop_3{};
};

// Duplicate
void set_object_properties_1(Object& object)
{
    if (object.prop_1 == 0) {
        object.prop_1 = 1;
    }

    object.prop_2 = object.prop_1 * 4;  // duplicate

    object.prop_3 = object.prop_2 * 5 + object.prop_1 * 2;
}

void set_object_properties_2(Object& object)
{
    if (object.prop_1 == 0) {
        object.prop_1 = 3;
    }
    object.prop_1 *= 2;

    object.prop_2 = object.prop_1 * 4;  // duplicate

    object.prop_3 = object.prop_1 * object.prop_2;
}

// Not recommended
void init_prop_1(Object& object, int type)
{
    switch (type) {
        case 1:
            if (object.prop_1 == 0) {
                object.prop_1 = 1;
            }
            break;
        case 2:
            if (object.prop_1 == 0) {
                object.prop_1 = 3;
            }
            object.prop_1 *= 2;
            break;
        default:
            // How do you handle this case?
            // The responsibility is yours.
            break;
    }
}

void init_prop_3(Object& object, int type)
{
    switch (type) {
        case 1:
            object.prop_3 = object.prop_2 * 5 + object.prop_1 * 2;
            break;
        case 2:
            object.prop_3 = object.prop_1 * object.prop_2;
            break;
        default:
            // How do you handle this case?
            // The responsibility is yours.
            break;
    }
}

void set_object_properties_by_type(Object& object, int type)
{
    init_prop_1(object, type);

    object.prop_2 = object.prop_1 * 4;

    init_prop_3(object, type);
}

// Better
struct init_prop_1a {
    static void init(Object& object)
    {
        if (object.prop_1 == 0) {
            object.prop_1 = 1;
        }
    }
};

struct init_prop_1b {
    static void init(Object& object)
    {
        if (object.prop_1 == 0) {
            object.prop_1 = 3;
        }
        object.prop_1 *= 2;
    }
};

struct init_prop_3a {
    static void init(Object& object) { object.prop_3 = object.prop_2 * 5 + object.prop_1 * 2; }
};

struct init_prop_3b {
    static void init(Object& object) { object.prop_3 = object.prop_1 * object.prop_2; }
};

template <typename prop_1_policy, typename prop_3_policy>
struct set_object_properties {
    static void set(Object& object)
    {
        prop_1_policy::init(object);

        object.prop_2 = object.prop_1 * 4;

        prop_3_policy::init(object);
    }
};

using set_object_1_properties = set_object_properties<init_prop_1a, init_prop_3a>;
using set_object_2_properties = set_object_properties<init_prop_1b, init_prop_3b>;

// Even better - configurable
struct init_config {
    int init{};
};

struct init_prop_1a_configurable {
    init_config& config_;

    void init(Object& object)
    {
        if (object.prop_1 == 0) {
            object.prop_1 = config_.init;
        }
    }
};

struct init_prop_1b_configurable {
    init_config& config_;

    void init(Object& object)
    {
        if (object.prop_1 == 0) {
            object.prop_1 = config_.init;
        }
        object.prop_1 *= 2;
    }
};

struct init_prop_3a_configurable {
    static void init(Object& object) { object.prop_3 = object.prop_2 * 5 + object.prop_1 * 2; }
};

struct init_prop_3b_configurable {
    static void init(Object& object) { object.prop_3 = object.prop_1 * object.prop_2; }
};

template <typename prop_1_policy, typename prop_3_policy>
struct set_object_properties_configurable : prop_1_policy, prop_3_policy {
    set_object_properties_configurable(init_config& config) : prop_1_policy{config}, prop_3_policy{} {}

    void set(Object& object)
    {
        prop_1_policy::init(object);

        object.prop_2 = object.prop_1 * 4;

        prop_3_policy::init(object);
    }
};

using set_object_1_properties_configurable =
    set_object_properties_configurable<init_prop_1a_configurable, init_prop_3a_configurable>;
using set_object_2_properties_configurable =
    set_object_properties_configurable<init_prop_1b_configurable, init_prop_3b_configurable>;

// Compose
using set_object_3_properties_configurable =
    set_object_properties_configurable<init_prop_1b_configurable, init_prop_3a_configurable>;

// Tag dispatching
struct Object1Tag {
};
void init_prop_1(Object1Tag, Object& object)
{
    if (object.prop_1 == 0) {
        object.prop_1 = 1;
    }
}

void init_prop_3(Object1Tag, Object& object) { object.prop_3 = object.prop_2 * 5 + object.prop_1 * 2; }

struct Object2Tag {
};
void init_prop_1(Object2Tag, Object& object)
{
    if (object.prop_1 == 0) {
        object.prop_1 = 3;
    }
    object.prop_1 *= 2;
}

void init_prop_3(Object2Tag, Object& object) { object.prop_3 = object.prop_1 * object.prop_2; }

template <typename Tag>
void set_object_properties_by_tag_dispatching(Tag tag, Object& object)
{
    init_prop_1(tag, object);

    object.prop_2 = object.prop_1 * 4;

    init_prop_3(tag, object);
}

// Inheritance
struct Object1 : Object {
};
void init_prop_1(Object1& object)
{
    if (object.prop_1 == 0) {
        object.prop_1 = 1;
    }
}

void init_prop_3(Object1& object) { object.prop_3 = object.prop_2 * 5 + object.prop_1 * 2; }

struct Object2 : Object {
};
void init_prop_1(Object2& object)
{
    if (object.prop_1 == 0) {
        object.prop_1 = 3;
    }
    object.prop_1 *= 2;
}

void init_prop_3(Object2& object) { object.prop_3 = object.prop_1 * object.prop_2; }

template <typename T>
void set_object_properties_by_child(T& object)
{
    init_prop_1(object);

    object.prop_2 = object.prop_1 * 4;

    init_prop_3(object);
}

int main()
{
    // Duplicate
    Object object_1{};
    set_object_properties_1(object_1);
    assert(object_1.prop_1 == 1);
    assert(object_1.prop_2 == 4);
    assert(object_1.prop_3 == 22);

    Object object_2{};
    set_object_properties_2(object_2);
    assert(object_2.prop_1 == 6);
    assert(object_2.prop_2 == 24);
    assert(object_2.prop_3 == 144);

    // Not recommended
    Object object_1b{};
    set_object_properties_by_type(object_1b, 1);
    assert(object_1b.prop_1 == 1);
    assert(object_1b.prop_2 == 4);
    assert(object_1b.prop_3 == 22);

    Object object_2b{};
    set_object_properties_by_type(object_2b, 2);
    assert(object_2b.prop_1 == 6);
    assert(object_2b.prop_2 == 24);
    assert(object_2b.prop_3 == 144);

    // Better
    Object object_1c{};
    set_object_1_properties::set(object_1c);
    assert(object_1c.prop_1 == 1);
    assert(object_1c.prop_2 == 4);
    assert(object_1c.prop_3 == 22);

    Object object_2c{};
    set_object_2_properties::set(object_2c);
    assert(object_2c.prop_1 == 6);
    assert(object_2c.prop_2 == 24);
    assert(object_2c.prop_3 == 144);

    // Even better - configurable
    Object object_1d{};

    init_config object_1_config{};
    object_1_config.init = 1;

    set_object_1_properties_configurable object_1_setter{object_1_config};
    object_1_setter.set(object_1d);

    assert(object_1d.prop_1 == 1);
    assert(object_1d.prop_2 == 4);
    assert(object_1d.prop_3 == 22);

    Object object_2d{};

    init_config object_2_config{};
    object_2_config.init = 3;

    set_object_2_properties_configurable object_2_setter{object_2_config};
    object_2_setter.set(object_2d);

    assert(object_2d.prop_1 == 6);
    assert(object_2d.prop_2 == 24);
    assert(object_2d.prop_3 == 144);

    // Compose
    Object object_3d{};

    init_config object_3_config{};
    object_3_config.init = 6;

    set_object_3_properties_configurable object_3_setter{object_3_config};
    object_3_setter.set(object_3d);

    assert(object_3d.prop_1 == 12);
    assert(object_3d.prop_2 == 48);
    assert(object_3d.prop_3 == 264);

    // Tag dispatching
    Object object_1e{};
    set_object_properties_by_tag_dispatching(Object1Tag{}, object_1e);
    assert(object_1e.prop_1 == 1);
    assert(object_1e.prop_2 == 4);
    assert(object_1e.prop_3 == 22);

    Object object_2e{};
    set_object_properties_by_tag_dispatching(Object2Tag{}, object_2e);
    assert(object_2e.prop_1 == 6);
    assert(object_2e.prop_2 == 24);
    assert(object_2e.prop_3 == 144);

    // Inheritance
    Object1 object_1f{};
    set_object_properties_by_child(object_1f);
    assert(object_1f.prop_1 == 1);
    assert(object_1f.prop_2 == 4);
    assert(object_1f.prop_3 == 22);

    Object2 object_2f{};
    set_object_properties_by_child(object_2f);
    assert(object_2f.prop_1 == 6);
    assert(object_2f.prop_2 == 24);
    assert(object_2f.prop_3 == 144);
}
