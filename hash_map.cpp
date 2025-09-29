#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

// This program implements a hash map keyed by integers and backed by an open-addressed table
// with linear probing. Load factor is kept under 60% by rehashing into a larger table, which
// preserves O(1) average insert, lookup, and removal while avoiding excessive clustering.
class HashMap {
public:
    explicit HashMap(std::size_t initialCapacity = 8) {
        if (initialCapacity < 8) {
            initialCapacity = 8;
        }
        // Buckets form the underlying fixed-size array for open addressing; each element
        // records a state flag so we can distinguish empty, occupied, and deleted slots.
        buckets.resize(initialCapacity);
    }

    void put(int key, const std::string& value) {
        // Growing before insertion keeps the load factor in the safe range for linear probing.
        ensureCapacity(count + 1);
        insertInternal(key, value);
    }

    bool get(int key, std::string& value) const {
        const Bucket* bucket = findBucket(key);
        if (!bucket) {
            return false;
        }
        value = bucket->value;
        return true;
    }

    bool contains(int key) const {
        return findBucket(key) != nullptr;
    }

    bool remove(int key) {
        if (buckets.empty()) {
            return false;
        }
        std::size_t index = indexFor(key);
        std::size_t start = index;
        while (true) {
            Bucket& bucket = buckets[index];
            if (bucket.state == State::Empty) {
                // Encountering a never-used slot means the key does not exist in the table.
                return false;
            }
            if (bucket.state == State::Occupied && bucket.key == key) {
                // Mark the slot as deleted to keep probing chains intact for other keys.
                bucket.state = State::Deleted;
                bucket.value.clear();
                --count;
                return true;
            }
            index = (index + 1) % buckets.size();
            if (index == start) {
                return false;
            }
        }
    }

    std::size_t size() const {
        return count;
    }

private:
    enum class State {
        Empty,
        Occupied,
        Deleted
    };

    struct Bucket {
        // key/value store the entry payload; state tracks whether the slot is free, occupied, or a tombstone.
        int key = 0;
        std::string value;
        State state = State::Empty;
    };

    std::vector<Bucket> buckets;
    std::size_t count = 0;

    std::size_t indexFor(int key) const {
        // std::hash<int> gives a well-distributed hash; mod by capacity selects a home bucket.
        return std::hash<int>{}(key) % buckets.size();
    }

    double loadFactor() const {
        if (buckets.empty()) {
            return 0.0;
        }
        return static_cast<double>(count) / static_cast<double>(buckets.size());
    }

    void ensureCapacity(std::size_t desired) {
        if (buckets.empty()) {
            buckets.resize(8);
            return;
        }
        double projected = static_cast<double>(desired) / static_cast<double>(buckets.size());
        if (projected > 0.6) {
            // Doubling capacity keeps probe sequences short and limits clustering.
            rehash(buckets.size() * 2);
        }
    }

    void insertInternal(int key, const std::string& value) {
        std::size_t index = indexFor(key);
        std::size_t firstDeleted = buckets.size();
        while (true) {
            Bucket& bucket = buckets[index];
            if (bucket.state == State::Empty) {
                // Either claim the empty slot or reuse a previously deleted one for better locality.
                Bucket* target = &bucket;
                if (firstDeleted != buckets.size()) {
                    target = &buckets[firstDeleted];
                }
                target->key = key;
                target->value = value;
                target->state = State::Occupied;
                ++count;
                return;
            }
            if (bucket.state == State::Deleted) {
                // Remember the first deleted slot so we can recycle it if the key is new.
                if (firstDeleted == buckets.size()) {
                    firstDeleted = index;
                }
            } else if (bucket.key == key) {
                // Existing key: overwrite in place to preserve the probing chain.
                bucket.value = value;
                return;
            }
            index = (index + 1) % buckets.size();
        }
    }

    void rehash(std::size_t newCapacity) {
        std::vector<Bucket> oldBuckets = std::move(buckets);
        buckets = std::vector<Bucket>(newCapacity);
        count = 0;
        for (const Bucket& bucket : oldBuckets) {
            if (bucket.state == State::Occupied) {
                // Reinserting via insertInternal recomputes the proper home position for the new table size.
                insertInternal(bucket.key, bucket.value);
            }
        }
    }

    const Bucket* findBucket(int key) const {
        if (buckets.empty()) {
            return nullptr;
        }
        std::size_t index = indexFor(key);
        std::size_t start = index;
        while (true) {
            const Bucket& bucket = buckets[index];
            if (bucket.state == State::Empty) {
                // An empty slot terminates the search—no later bucket can contain the key.
                return nullptr;
            }
            if (bucket.state == State::Occupied && bucket.key == key) {
                return &bucket;
            }
            index = (index + 1) % buckets.size();
            if (index == start) {
                // We looped through the entire table without success.
                return nullptr;
            }
        }
    }
};

int main() {
    HashMap phoneBook;

    phoneBook.put(5551234, "Alice");
    phoneBook.put(5552345, "Bob");
    phoneBook.put(5553456, "Carol");
    phoneBook.put(5551234, "Alice Cooper");

    std::string contact;
    if (phoneBook.get(5551234, contact)) {
        std::cout << "Updated 5551234 -> " << contact << "\n";
    }
    if (phoneBook.get(5553456, contact)) {
        std::cout << "Found 5553456 -> " << contact << "\n";
    }

    phoneBook.remove(5552345);
    std::cout << "Contains 5552345? " << (phoneBook.contains(5552345) ? "yes" : "no") << "\n";
    std::cout << "Map holds " << phoneBook.size() << " entries\n";

    for (int key : {5551234, 5552345, 5553456}) {
        // Probe a few keys to show how lookups succeed, miss, and reflect removals.
        if (phoneBook.get(key, contact)) {
            std::cout << key << " -> " << contact << "\n";
        } else {
            std::cout << key << " is not present\n";
        }
    }

    return 0;
}
