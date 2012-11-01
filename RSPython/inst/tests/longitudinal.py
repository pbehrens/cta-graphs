
class Longitudinal:
    times = ()
    vals = ();
    def __init__(self, Times, Vals):
      self.times = Times
      self.vals = Vals
      print "Ok"
      return(None)
    def length(self):
        ""
        return(len(self.times))
