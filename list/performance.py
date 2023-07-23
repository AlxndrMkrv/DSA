import matplotlib.pyplot as plt
import sys
from timeit import timeit

sys.path.append(sys.argv[-1])

import list_nb
import list_pb

class PyList (list):
    def __init__(self, n: int = 0):
        if n:
            list.__init__(self, (0 for _ in range(n)))
        else:
            list.__init__(self)


TestObjects = {"list": PyList, 
               "MyList_nano": list_nb.MyList,
               "MyList_pb11": list_pb.MyList,
               "StlList_nano": list_nb.StlList,
               "StlList_pb11": list_pb.StlList
}

class Performance:
    def __init__(self, max_value: int = 1000, steps_number: int = 1000):
        self.x = [max_value := int(max_value * 0.95)
                  for _ in range(steps_number) if max_value > 1][::-1]

    def measure_append_time(self, TList, replay: int = 10):
        return [timeit(f"[l.append(0) for i in range({x})]", number=replay, 
                       globals={'l': TList()}) for x in self.x]

    def measure_get_time(self, TList, replay: int = 10):
        return [timeit(f"l[{int(x/2)}]", number=replay,
                       globals={'l': TList(x)}) for x in self.x]

    def measure_size_time(self, TList, replay: int = 10):
        return [timeit(f"len(l)", number=replay,
                       globals={'l': TList(x)}) for x in self.x]

    def measure_insert_time(self, TList, replay: int = 10):
        return [timeit(f"l.insert({int(x/2)}, 0)", number=replay,
                       globals={'l': TList(x)}) for x in self.x]

    def measure_pop_time(self, TList, replay: int = 10):
        return [timeit(f"l.pop({int(x/2)}); l.insert({int(x/2)}, 0)", number=replay,
                       globals={'l': TList(x)}) for x in self.x]
    
    def plot(self):
        fix, ax = plt.subplots(2, 3, sharex=True)
        for i, j, title, method in [(0, 0, "Append(x)", self.measure_append_time),
                                    (1, 0, "Get middle element", self.measure_get_time),
                                    (0, 1, "Size", self.measure_size_time),
                                    (1, 1, "Insert to middle", self.measure_insert_time),
                                    (0, 2, "Pop from middle", self.measure_pop_time)
                                    ]:
            ax[i][j].set_title(title)
            for lbl, obj in TestObjects.items():
                ax[i][j].plot(self.x,
                              method(obj),
                              label=lbl)
            ax[i][j].legend()
            ax[i][j].grid()

        plt.tight_layout()
        plt.show()

Performance(max_value=1000).plot()

