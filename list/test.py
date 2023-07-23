from importlib import import_module
from types import ModuleType
import pytest

# import modules to be tested
import list_nb as nb
import list_pb as pb


dep = pytest.mark.dependency
prm = pytest.mark.parametrize


all_lists = [pytest.param(getattr(globals()[mod], list_type),
                          marks=dep(depends=[mod]))
             for mod, list_type in [("nb", "StlList"),
                                    ("nb", "MyList"),
                                    ("pb", "StlList"),
                                    ("pb", "MyList")]]


@prm("module", [pytest.param(nb, marks=dep(name="nb")),
                pytest.param(pb, marks=dep(name="pb"))])
def test_exported_types(module):
    assert all(hasattr(module, attr)
               for attr in ["MyList", "StlList"])


# constructors
@prm("TList", all_lists)
def test_constructor(TList):
    assert isinstance(TList(), TList)
    assert isinstance(TList(100), TList)

# __len__()
@prm("TList", all_lists)
def test_len(TList):
    assert len(TList()) == 0
    assert len(TList(55)) == 55

# append()
@prm("TList", all_lists)
def test_append(TList):
    lst = TList()
    [lst.append(i) for i in range(5)]
    assert len(lst) == 5

# __repr__()
@prm("TList", all_lists)
def test_repr(TList):
    lst = TList()
    rng = range(4, 0, -1)
    [lst.append(i) for i in rng]
    assert str(lst) == str([i for i in rng])

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
    lst = TList(length)
    for i in rng:
        lst[i] = i
    assert str(lst) == str([i for i in rng])

# insert()
@prm("TList", all_lists)
def test_insert(TList):
    lst = TList(2)
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
    except Exception as exc:
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


