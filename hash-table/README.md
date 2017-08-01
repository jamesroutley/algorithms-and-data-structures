# Hash table

An implementation of a hash table, using open addressing with double hashing to
handle collisions.


## Analytics

Analytics are written in Python 3, using the `ctypes` module to interface with
the `C` hash table. To do this, compile the hash table to a shared library with:

```
$ make build
```

Set up the python environment:

```
$ cd analytics
$ python3 -m venv env
$ source env/bin/activate
$ pip install -r requirements.txt
```

You should then be able to run the analytics scripts with:

```
$ python <name of script>.py
```

The available scripts are:

- `plot_insert_time.py`: plots the time taken to insert 10000 items
- `plot_delete_time.py`: inserts 10000 items, then plots the time taken to
  delete them.
- `scale_up_down.py`: tests the hash table with a large load by inserting,
  searching for and deleting 100000 items.
