#pragma once

// Requires operation T + T, for usage of +=
template <typename C, typename T>
class Property {
private:
    // T &_value;
    T &_value;
    C &_obj;
    void (C::*callback)();

public:
    Property(C &obj, T &value, void (C::*callback)()) : _obj(obj), _value(value) {
        this->callback = callback;
    } // eo ctor

    Property<C, T> &operator=(const T &val) {
        _value = val;
        (_obj.*callback)();
        //(*onchangecallback)(_value);
        return *this;
    }; // eo operator =

    Property<C, T> &operator+=(const T &other) {
        // base = this->value + other;
        (*this) = this->_value + other;
        return *this;
    }

    operator const T &() const {
        return _value;
    }; // eo operator ()
};

// Requires operation T + T, for usage of +=
// arg1: Caller object.
// arg2: = function
// arg3: += function
// arg4: get function (cast to T)
template <typename C, typename T>
class GetterSetter {
private:
    // T &_value;
    C &_obj;
    void (C::*eq)(const T &);
    void (C::*add)(const T &);
    const T (C::*getter)();

public:
    // arg1: Caller object.
    // arg2: = function
    // arg3: += function
    // arg4: get function (cast to T)
    GetterSetter(C &obj, void (C::*eq)(const T &), void (C::*add)(const T &), const T (C::*getter)()) : _obj(obj) {
        this->eq = eq;
        this->getter = getter;
        this->add = add;
    } // eo ctor

    GetterSetter<C, T> &operator=(const T &val) {
        //_value = val;
        //(_obj.*callback)();
        //(*onchangecallback)(_value);
        (_obj.*eq)(val);
        return *this;
    }; // eo operator =

    GetterSetter<C, T> &operator+=(const T &other) {
        // base = this->value + other;
        // (*this) = this->_value + other;
        //(_obj.*setter)((_obj.*getter()) + other);
        (_obj.*add)(other);
        return *this;
    }

    operator const T() const {
        return (_obj.*getter)();
    }; // eo operator ()
};