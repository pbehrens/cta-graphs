import RS

class R(object):
     def __get__(self, obj, klass = None):
          def wrapper(*args, **keywordargs):
               return RS.call(obj, *args, **keywordargs)
          return wrapper        

     @staticmethod
     def __getattribute__(name):
          def wrapper(*args, **keywordargs):
                   return RS.call(name, *args, **keywordargs)
          return wrapper


#     def __getattribute__(obj, name):
#          def wrapper(*args, **keywordargs):
#                   return RS.call(name, *args, **keywordargs)
#          return wrapper

# e.g.
#   from R import R
#   R.rnorm(20)
# or
#   from R import R as Rengine
#   Rengine.rnorm(20)

R = R()

