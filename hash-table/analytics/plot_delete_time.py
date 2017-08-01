import timeit

from bokeh.plotting import figure, output_file, show

import ht


def delete(s):
    ht.delete(hash_table, bytes(s, "utf-8"))


def plot(x, y):
    output_file = "graphs/delete_time.html"
    p = figure(
        title="number of deleted items vs delete time",
        x_axis_label="number of deleted items",
        y_axis_label="delete time (seconds)")
    p.line(x, y, line_width=2)
    show(p)


if __name__ == "__main__":
    num_items = 10000

    hash_table = ht.new()

    for i in range(num_items):
        ht.insert(hash_table, bytes(str(i), "utf-8"), b"value")

    x = [None] * num_items
    y = [None] * num_items
    for i in range(num_items):
        result = timeit.timeit(
            "delete('{0}')".format(i),
            setup="from __main__ import delete",
            number=1)
        x[i]= i
        y[i] = result

    plot(x, y)
