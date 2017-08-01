import timeit

from bokeh.plotting import figure, output_file, show

import ht


def insert(s):
    ht.insert(hash_table, bytes(s, "utf-8"), b"value")


def plot(x, y):
    output_file = "graphs/insert_time.html"
    p = figure(
        title="number of inserted items vs insert time",
        x_axis_label="number of inserted items",
        y_axis_label="insert time (seconds)")
    p.line(x, y, line_width=2)
    show(p)


if __name__ == "__main__":
    num_inserts = 10000

    hash_table = ht.new()

    x = [None] * num_inserts
    y = [None] * num_inserts
    for i in range(num_inserts):
        result = timeit.timeit(
            "insert('{0}')".format(i),
            setup="from __main__ import insert",
            number=1)
        x[i]= i
        y[i] = result

    plot(x, y)
