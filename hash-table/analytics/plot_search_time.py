import timeit

from bokeh.plotting import figure, output_file, show

import ht


def search(s):
    ht.search(hash_table, bytes(s, "utf-8"))


def plot(x, y):
    output_file = "graphs/search_time.html"
    p = figure(
        title="number of searched items vs search time",
        x_axis_label="number of searched items",
        y_axis_label="search time (seconds)")
    p.line(x, y, line_width=2)
    show(p)


if __name__ == "__main__":
    num_items = 500

    hash_table = ht.new()

    for i in range(num_items):
        ht.insert(hash_table, bytes(str(i), "utf-8"), b"value")

    x = [None] * num_items
    y = [None] * num_items
    for i in range(num_items):
        total_time = 0
        for j in range(i + 1):
            total_time +=  timeit.timeit(
                "search('{0}')".format(j),
                setup="from __main__ import search",
                number=5)
        average_time = total_time / (i + 1)
        x[i]= i
        y[i] = average_time

    plot(x, y)
