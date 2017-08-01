import ht


if __name__ == "__main__":
    num_items = 100000

    hash_table = ht.new()

    for i in range(num_items):
        ht.insert(hash_table, bytes(str(i), "utf-8"), b"value")

    for i in range(num_items):
        ht.search(hash_table, bytes(str(i), "utf-8"), b"value")

    for i in range(num_items):
        ht.delete(hash_table, bytes(str(i), "utf-8"), b"value")

    ht.del_hash_table(hash_table)

