from ctypes import *
import os


class item(Structure):
    _fields_ = [
        ("key", c_char_p),
        ("value", c_char_p),
    ]


class hash_table(Structure):
    _fields_ = [
        ("size_index", c_int),
        ("size", c_int),
        ("load", c_int),
        ("items", POINTER(POINTER(item)))
    ]


def _load_ht():
    root_dir = os.path.dirname(os.path.dirname(__file__))
    lib = cdll.LoadLibrary(os.path.join(root_dir, "build", "hash_table.so"))

    new = lib.ht_new
    new.restype = POINTER(hash_table)

    insert = lib.ht_insert
    insert.argtypes = [POINTER(hash_table), c_char_p, c_char_p]

    search = lib.ht_search
    search.argtypes = [POINTER(hash_table), c_char_p]
    search.restype = c_char_p

    delete = lib.ht_delete
    delete.argtypes = [POINTER(hash_table), c_char_p]

    del_hash_table = lib.ht_del_hash_table
    del_hash_table.argtypes = [POINTER(hash_table)]

    return new, insert, search, delete, del_hash_table


new, insert, search, delete, del_hash_table = _load_ht()
