import pytest

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
    assert hasattr(module, "StlArray")
