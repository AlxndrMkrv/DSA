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
               "StlList_pb11": list_pb.StlList}

class Performance:
    def __init__(self, max_value: int = 1000, steps_number: int = 1000):
        self.x = [max_value := int(max_value * 0.95)
                  for _ in range(steps_number) if max_value > 1][::-1]

    def measure_append_time(self, TList, replay: int = 10):
        return [timeit(f"[l.append(0) for i in range({x})]", number=replay, 
                       globals={'l': TList()}) for x in self.x]

    def measure_get_time(self, TList, replay: int = 10):
        return [timeit(f"l[{round(x-1)}]", number=replay, 
                       globals={'l': TList(x)}) for x in self.x]
    
    def plot(self):
        fix, ax = plt.subplots(2, 2, sharex=True)
        ax[0][0].set_title("Append time")
        for lbl, obj in TestObjects.items():
            ax[0][0].plot(self.x, 
                          self.measure_append_time(obj), 
                          label=lbl)
        ax[0][0].legend()
        ax[0][0].grid()

        ax[1][0].set_title("Get time")
        for lbl, obj in TestObjects.items():
            ax[1][0].plot(self.x,
                          self.measure_get_time(obj),
                          label=lbl)
        ax[1][0].legend()
        ax[1][0].grid()

        plt.tight_layout()
        plt.show()

Performance().plot()




