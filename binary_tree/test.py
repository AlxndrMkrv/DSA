import pytest
import random

# import tested modules
import binary_tree_nb as nb
import binary_tree_pb as pb


dep = pytest.mark.dependency
prm = pytest.mark.parametrize

test_sequence = [5, 2, 10, 1, 10, 5, 8, 4, 7, 9, 7, 3, 9, 15, 11, 20]

all_trees = [pytest.param(getattr(globals()[mod], "Tree"),
                          marks=dep(depends=[mod]))
             for mod in ["nb", "pb"]]


@prm("module", [pytest.param(nb, marks=dep(name="nb")),
                pytest.param(pb, marks=dep(name="pb"))])
def test_exported_types(module):
    assert all(hasattr(module, attr) for attr in ["Tree"])


@prm("T", all_trees)
def test_constructor(T, length: int = 1000):
    assert isinstance(T(), T)
    assert isinstance(T([random.randint(0, length*10)
                         for _ in range(length)]), T)

@prm("T", all_trees)
def test_len(T, length: int = 1000):
    tree = T([_ for _ in range(length)])
    assert len(tree) == length


@prm("T", all_trees)
def test_clear(T, length: int = 1000):
    tree = T([_ for _ in range(length)])
    assert len(tree) == length
    tree.clear()
    assert len(tree) == 0


@prm("T", all_trees)
def test_add(T, length: int = 1000):
    tree = T()
    [tree.add(_) for _ in range(length)]
    assert len(tree) == length


@prm("T", all_trees)
def test_contains(T, length: int = 100):
    tree = T()
    [tree.add(i) for i in range(length)]
    assert 0 in tree
    assert length-1 in tree
    assert length not in tree


@prm("T", all_trees)
def test_remove(T):
    tree = T(test_sequence)
    assert len(tree) == len(set(test_sequence))
    tree.remove(10)
    assert len(tree) == len(set(test_sequence)) - 1

    try:
        tree.remove(999)
    except Exception as exc:
        assert isinstance(exc, KeyError)

    tree = T([0])
    tree.remove(0)
    try:
        tree.remove(0)
    except Exception as exc:
        assert isinstance(exc, KeyError)


@prm("T", all_trees)
def test_repr(T, sequence: set = set(test_sequence)):
    assert isinstance(sequence, set)
    tree = T(sequence)
    assert tree.__repr__() == sequence.__repr__()


@prm("T", all_trees)
def test_pop(T, sequence: set = set(test_sequence)):
    assert isinstance(sequence, set)
    tree = T(sequence)
    assert tree.__repr__() == sequence.__repr__()
    sequence.pop()
    tree.pop()
    assert len(tree) == len(sequence)
    assert tree.__repr__() == sequence.__repr__()

    for seq in [[1,2,3], [2,1,3], [3,2,1], [1]]:
        tree = T(seq)
        assert tree.pop() == 1
        assert len(tree) == (len(seq) - 1)

    tree = T()
    try:
        tree.pop()
    except Exception as exc:
        assert isinstance(exc, KeyError)



