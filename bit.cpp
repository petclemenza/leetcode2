#include <iostream>
#include <vector>
#include <numeric>

class BIT {
    std::vector<int> _data;
    std::vector<int> _tree;

    int sum(int i) const {
        int s = 0;
        while (i > 0) {
            s += _tree[i];
            i -= (i & -i);
        }

        return s;
    }
public:

    BIT(const std::vector<int>& data)
    {
        _data.resize(data.size(), 0);
        _tree.resize(data.size() + 1);
        for (size_t i = 0; i < data.size(); ++i) {
            update(i, data[i]);
        }
    }

    int sum(int l, int r) const {
        return sum(r + 1) - sum(l);
    }

    void update(int i, int val) {
        int del = val - _data[i];
        _data[i++] = val;

        while (i < static_cast<int>(_tree.size())) {
            _tree[i] += del;
            i += (i & -i);
        }

    }
};

class BIT2D {
    std::vector<std::vector<int>> _data;
    std::vector<std::vector<int>> _tree;
    int _max_x;
    int _max_y;

    int sum_y(std::pair<int, int> i) const {
        int s = 0;
        int y = i.second;
        while (y > 0) {
            s += _tree[i.first][y];
            y -= (y & -y);
        }

        return s;
    }

    int sum(std::pair<int, int> i) const {
        int s = 0;
        int x = i.first;
        while (x > 0) {
            s += sum_y({x, i.second});
            x -= (x & -x);
        }

        return s;
    }

    void update_y(std::pair<int, int> i, int val) {
        int y = i.second;
        while (y < _max_y) {
            _tree[i.first][y] += val;
            y += (y & -y);
        }
    }

public:

    BIT2D(const std::vector<std::vector<int>>& data)
        : _data(data), _max_x(static_cast<int>(data.size() + 1)), _max_y(1)
    {
        if (_max_x > 1) {
            _max_y = static_cast<int>(data.front().size() + 1);
        }

        _tree.resize(_max_x, std::vector<int>(_max_y, 0));

        for (int x = 0; x < static_cast<int>(data.size()); ++x) {
            for (int y = 0; y < static_cast<int>(data.front().size()); ++y) {
                _data[x][y] = 0;
                update({x, y}, data[x][y]);
            }
        }
    }

    int sum(std::pair<int, int> l, std::pair<int, int> r) const
    {
        return sum({r.first + 1, r.second + 1}) + sum(l) - sum({r.first + 1, l.second}) - sum({l.first, r.second + 1});
    }

    void update(std::pair<int, int> i, int val)
    {
        int del = val - _data[i.first][i.second];
        _data[i.first][i.second] += del;

        int x = i.first + 1;
        while (x < _max_x) {
            update_y({x, i.second + 1}, del);
            x += (x & -x);
        }
    }
};

int main()
{
    /*
    BIT2D bit({{3, 0, 1, 4, 2}, {5, 6, 3, 2, 1}, {1, 2, 0, 1, 5}, {4, 1, 0, 1, 7}, {1, 0, 3, 0, 5}});
    std::cout << bit.sum({2,1}, {4,3}) << std::endl;
    bit.update({3, 2}, 2);
    std::cout << bit.sum({2,1}, {4,3}) << std::endl;

    std::cout << bit.sum({0,0}, {0,0}) << std::endl;
    */

    BIT2D bit({{1}, {2}});
    std::cout << bit.sum({0,0}, {0,0}) << std::endl;
    std::cout << bit.sum({1,0}, {1,0}) << std::endl;

/*
    BIT bit({1, 0, 2, 1, 1, 3, 0, 4, 2, 5, 2, 2, 3, 1, 0, 2});
    std::cout << bit.sum(0, 15) << std::endl;
    bit.update(2, 5);
    std::cout << bit.sum(0, 15) << std::endl;
    std::cout << bit.sum(2, 15) << std::endl;
    bit.update(2, 1);
    std::cout << bit.sum(2, 15) << std::endl;
*/
    return 0;
}