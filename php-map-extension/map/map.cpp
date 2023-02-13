#include <phpcpp.h>
#include <map>

class Map : public Php::Base
{
    private:
        /**
         * The container to hold map data
         * @var array
         */
        std::map<Php::Value, Php::Value> _container;

    public:
        /**
         * Constructor and destructor
         */
         Map() = default;
         virtual ~Map() = default;

        /**
         * Set method
         */
         void set(Php::Parameters &params) {
            _container[params[0]] = params[1];
         }

        /**
         * Get method
         */
         Php::Value get(Php::Parameters &params) {
            return _container[params[0]];
         }

        /**
         * Has method
         */
         Php::Value has(Php::Parameters &params) {
            return _container.find(params[0]) != _container.end();
         }

        /**
         * Delete method
         */
         void del(Php::Parameters &params) {
            _container.erase(params[0]);
         }

        /**
         * Length method
         */
         Php::Value length() {
           int64_t count = _container.size();
           return count;
         }

        /**
         * Range method
         */
         Php::Value range(Php::Parameters &params) {
            Php::Value func = params[0];
            Php::Array items;

            for (auto &element : _container) {
                if (func(element.second, element.first) == true) {
                    items[element.first] = element.second;
                }
            }

            return items;
         }
};

extern "C" {
    PHPCPP_EXPORT void *get_module() {
        static Php::Extension ext("map", "1.0");

        // Make methods accessible

        Php::Class<Map> map("Map");

        map.method<&Map::set> ("set", {
            Php::ByVal("key", Php::Type::String, true),
            Php::ByVal("value", Php::Type::Null, true)
        });

        map.method<&Map::get> ("get", {
            Php::ByVal("key", Php::Type::String, true)
        });

        map.method<&Map::has> ("has", {
            Php::ByVal("key", Php::Type::String, true)
        });

        map.method<&Map::del> ("delete", {
            Php::ByVal("key", Php::Type::String, true)
        });

        map.method<&Map::length> ("length");

        map.method<&Map::range> ("range", {
            Php::ByVal("callback", Php::Type::Callable, true)
        });

        // Add class to extension
        ext.add(std::move(map));

        return ext;
    }
}
