import pytest
import random


# import tested modules
import binary_tree_nb as nb
import binary_tree_pb as pb


dep = pytest.mark.dependency
prm = pytest.mark.parametrize

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
    tree = T([5, 2, 10, 1, 10, 5, 8, 4, 7, 9, 7, 3, 9, 15, 11, 20])
    assert len(tree) == 12
    tree.remove(10)
    assert len(tree) == 11

