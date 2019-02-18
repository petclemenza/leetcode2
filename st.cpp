#include <iostream>
#include <vector>
#include <numeric>

class ST {

    struct Node {
        size_t index;
        size_t left;
        size_t right;
    };

    std::vector<int> _tree;
    size_t _sz;

    void build(const Node& node, const std::vector<int> data)
    {
        if (node.left == node.right) {
            _tree[node.index] = data[node.left];
        } else {
            size_t middle = node.left + (node.right - node.left) / 2;

            build({node.index * 2, node.left, middle}, data);
            build({node.index * 2 + 1, middle + 1, node.right}, data);

            _tree[node.index] = _tree[node.index * 2] + _tree[node.index * 2 + 1];
        }
    }

    void update(const Node& node, size_t i, int val)
    {
        if (i < node.left || i > node.right) {
            return;
        }

        if (node.right > node.left) {
            size_t middle = node.left + (node.right - node.left) / 2;
            if (i <= middle) {
                update({node.index * 2, node.left, middle}, i, val);
            } else {
                update({node.index * 2 + 1, middle + 1, node.right}, i, val);
            }
            _tree[node.index] = _tree[node.index * 2] + _tree[node.index * 2 + 1];
        } else {
            _tree[node.index] = val;
        }
    }

    int sum(const Node& node, size_t l, size_t r) const
    {
        if (l > r) {
            return 0;
        }

        if (node.left == l && node.right == r) {
            return _tree[node.index];
        }

        size_t middle = node.left + (node.right - node.left) / 2;
        int s = 0;
        s += sum({node.index * 2, node.left, middle}, l, std::min<size_t>(r, middle));
        s += sum({node.index * 2 + 1, middle + 1, node.right}, std::max<size_t>(middle + 1, l), r);
        return s;
    }

public:

    ST(const std::vector<int>& data) : _sz(data.size())
    {
        if (_sz) {
            _tree.resize(4 * _sz);
            build({1, 0, _sz - 1}, data);
        }
    }

    int sum(size_t l, size_t r) const
    {
        if (!_sz) {
            return 0;
        }
        return sum({1, 0, _sz - 1}, l, r);
    }

    void update(size_t i, int val)
    {
        if (!_sz) {
            return;
        }

        update({1, 0, _sz - 1}, i, val);
    }
};

class ST2D {

    struct Node {
        size_t index;
        size_t left;
        size_t right;
    };

    struct Node2D {
        Node x;
        Node y;
    };

    std::vector<std::vector<int>> _tree;
    size_t _x_sz;
    size_t _y_sz;

    void build_y(const Node2D& node, const std::vector<std::vector<int>>& data)
    {
        if (node.y.left == node.y.right) {

            if (node.x.left == node.x.right) {
                _tree[node.x.index][node.y.index] = data[node.x.left][node.y.left];
            } else {
                _tree[node.x.index][node.y.index] = 0;
                _tree[node.x.index][node.y.index] += _tree[node.x.index * 2][node.y.index];
                _tree[node.x.index][node.y.index] += _tree[node.x.index * 2 + 1][node.y.index];
            }

        } else {

            size_t middle = node.y.left + (node.y.right - node.y.left) / 2;
            build_y(
                {{node.x}, {node.y.index * 2, node.y.left, middle}},
                data
                );
            build_y(
                {{node.x}, {node.y.index * 2 + 1, middle + 1, node.y.right}},
                data
                );

            _tree[node.x.index][node.y.index] = 0;
            _tree[node.x.index][node.y.index] += _tree[node.x.index][node.y.index * 2];
            _tree[node.x.index][node.y.index] += _tree[node.x.index][node.y.index * 2 + 1];
        }
    }

    void build_x(const Node2D& node, const std::vector<std::vector<int>>& data)
    {
        if (node.x.left != node.x.right) {
            size_t middle = node.x.left + (node.x.right - node.x.left) / 2;
            build_x({{node.x.index * 2, node.x.left, middle}, node.y}, data);
            build_x({{node.x.index * 2 + 1, middle + 1, node.x.right}, node.y}, data);
        }

        build_y(node, data);
    }

    int sum_y(const Node2D& node, size_t x_left, size_t x_right, size_t y_left, size_t y_right) const
    {
        if (y_left > y_right) {
            return 0;
        }

        if (node.y.left == y_left && node.y.right == y_right) {
            return _tree[node.x.index][node.y.index];
        }

        size_t middle = node.y.left + (node.y.right - node.y.left) / 2;

        int s = 0;
        s += sum_y(
            {node.x, {node.y.index * 2, node.y.left, middle}},
            x_left, x_right, y_left, std::min<size_t>(middle, y_right));
        s += sum_y(
            {node.x, {node.y.index * 2 + 1, middle + 1, node.y.right}},
            x_left, x_right, std::max<size_t>(middle + 1, y_left), y_right);
        return s;
    }

    int sum_x(const Node2D& node, size_t x_left, size_t x_right, size_t y_left, size_t y_right) const
    {
        if (x_left > x_right) {
            return 0;
        }

        if (node.x.left == x_left && node.x.right == x_right) {
            return sum_y(node, x_left, x_right, y_left, y_right);
        }

        size_t middle = node.x.left + (node.x.right - node.x.left) / 2;

        int s = 0;
        s += sum_x(
            {{node.x.index * 2, node.x.left, middle}, node.y},
            x_left, std::min<size_t>(middle, x_right), y_left, y_right
            );
        s += sum_x(
            {{node.x.index * 2 + 1, middle + 1, node.x.right}, node.y},
            std::max<size_t>(middle + 1, x_left), x_right, y_left, y_right
            );
        return s;
    }

    void update_y(const Node2D& node, size_t x, size_t y, int val)
    {
        if (node.y.left == node.y.right) {

            if (node.x.left == node.x.right) {
                _tree[node.x.index][node.y.index] = val;
            } else {
                _tree[node.x.index][node.y.index] = 0;
                _tree[node.x.index][node.y.index] += _tree[node.x.index * 2][node.y.index];
                _tree[node.x.index][node.y.index] += _tree[node.x.index * 2 + 1][node.y.index];
            }

        } else {

            size_t middle = node.y.left + (node.y.right - node.y.left) / 2;
            if (y <= middle) {
                update_y(
                    {node.x, {node.y.index * 2, node.y.left, middle}},
                    x, y, val
                    );
            } else {
                update_y(
                    {node.x, {node.y.index * 2 + 1, middle + 1, node.y.right}},
                    x, y, val
                    );
            }
            _tree[node.x.index][node.y.index] = 0;
            _tree[node.x.index][node.y.index] += _tree[node.x.index][node.y.index * 2];
            _tree[node.x.index][node.y.index] += _tree[node.x.index][node.y.index * 2 + 1];
        }
    }

    void update_x(const Node2D& node, size_t x, size_t y, int val)
    {
        if (node.x.right > node.x.left) {
            size_t middle = node.x.left + (node.x.right - node.x.left) / 2;
            if (x <= middle) {
                update_x(
                    {{node.x.index * 2, node.x.left, middle}, node.y},
                    x, y, val
                    );
            } else {
                update_x(
                    {{node.x.index * 2 + 1, middle + 1, node.x.right}, node.y},
                    x, y, val
                    );
            }
        }

        update_y(node, x, y, val);
    }
public:

    ST2D(const std::vector<std::vector<int>>& data) : _x_sz(data.size()), _y_sz(0)
    {
        if (_x_sz) {
            _y_sz = data.front().size();
        }

        if (_x_sz && _y_sz) {
            _tree.resize(4 * _x_sz, std::vector<int>(4 * _y_sz));
            build_x(
                {{1, 0, _x_sz - 1}, {1, 0, _y_sz - 1}},
                data
                );
        }
    }

    int sum(size_t x_left, size_t y_left, size_t x_right, size_t y_right) const
    {
        if (_x_sz && _y_sz) {
            return sum_x(
                    {{1, 0, _x_sz - 1}, {1, 0, _y_sz - 1}},
                    x_left, x_right, y_left, y_right
                    );
        }

        return 0;
    }

    void update(size_t x, size_t y, int val)
    {
        update_x(
            {{1, 0, _x_sz - 1}, {1, 0, _y_sz - 1}},
            x, y, val
            );
    }
};

int main()
{
    /*
    ST st({0,1,2,3,4,5,6,7,8});
    std::cout << st.sum(0, 8) << std::endl;
    st.update(0, 8);
    st.update(8, 0);
    std::cout << st.sum(0, 8) << std::endl;
    std::cout << st.sum(0, 0) << std::endl;
    st.update(0, 1);
    std::cout << st.sum(0, 0) << std::endl;
    */

    ST2D st({{0,1,2}, {3,4,5}, {6,7,8}});
    std::cout << st.sum(0, 0, 2, 2) << std::endl;
    st.update(0, 0, 1);
    std::cout << st.sum(0, 0, 2, 2) << std::endl;
    std::cout << st.sum(0, 0, 0, 0) << std::endl;
    return 0;
}