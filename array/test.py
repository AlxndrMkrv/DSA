import pytest
import random


import array_nb as nb
import array_pb as pb


dep = pytest.mark.dependency
prm = pytest.mark.parametrize


all_lists = [pytest.param(getattr(globals()[mod], "StlArray"),
                          marks=dep(depends=[mod]))
             for mod in ["nb", "pb"]]


@prm("module", [pytest.param(nb, marks=dep(name="nb")),
                pytest.param(pb, marks=dep(name="pb"))])
def test_exported_types(module):
    assert hasattr(module, "StlLit")


# constructors
@prm("TList", all_lists)
def test_constructor(TList):
    assert isinstance(TList, TList)
    assert isinstance(TList([1]), TList)

# __len__()
@prm("TList", all_lists)
def test_len(TList, n: int = random.choice(range(10, 100))):
    assert len(TList()) == 0
    assert len(TList(range(n))) == n

# append()
@prm("TList", all_lists)
def test_append(TList):
    lst = TList()
    lst.append(5)
    lst.append(0.1)
    lst.append("asdf")
    lst.append([1])
    lst.append(TList())
    assert len(lst) == 5

# __repr__()
@prm("TList", all_lists)
def test_repr(TList, n: int = random.choice(range(5, 10))):
    lst = TList()
    ref_list = [random.choice(range(1000)) for _ in range(n)]
    [lst.append(i) for i in ref_list]
    assert str(lst) == str(ref_list)

# __getitem__()
@prm("TList", all_lists)
def test_getitem(TList):
    lst = TList()
    [lst.append(i) for i in range(10)]
    assert all([lst[i] == i for i in range(10)])

# __setitem__()
@prm("TList", all_lists)
def test_setitem(TList, length: int = 5):
    rng = range(length)
    lst = TList(rng)
    for i in rng:
        lst[i] = i
    assert str(lst) == str([i for i in rng])

# insert()
@prm("TList", all_lists)
def test_insert(TList):
    lst = TList([0, 0])
    lst.insert(0, 1)
    assert str(lst) == str([1, 0, 0])
    assert len(lst) == 3
    lst.insert(99, 3)
    assert str(lst) == str([1, 0, 0, 3])
    assert len(lst) == 4
    lst.insert(2, 5)
    assert str(lst) == str([1, 0, 5, 0, 3])
    assert len(lst) == 5

# pop()
@prm("TList", all_lists)
def test_pop(TList):
    lst = TList()
    try:
        lst.pop()
        assert False
    except IndexError:
        assert True
    except:
        assert False
    [lst.append(i+1) for i in range(5)]
    assert lst.pop(2) == 3
    assert str(lst) == str([1, 2, 4, 5])
    assert len(lst) == 4
    assert lst.pop(0) == 1
    assert str(lst) == str([2, 4, 5])
    assert len(lst) == 3
    assert lst.pop() == 5
    assert str(lst) == str([2, 4])
    assert len(lst) == 2


'''
def test_implementation(TList):
    # default constructor
    assert isinstance(TList(), TList)

    # assign constructor
    assert isinstance(TList(10), TList)

    # __repr__()
    lst = TList()
    assert str(lst) == "[]"

    # append()
    [lst.append(i) for i in [1,2,3,4,5]]
    assert str(lst) == "[1, 2, 3, 4, 5]"

    # __len__()
    assert len(TList()) == 0
    assert len(TList(100)) == 100

    # __getitem__()
    for i, value in enumerate(lst):
        assert lst[i] == i+1 == value'''


