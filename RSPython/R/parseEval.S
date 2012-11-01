parseEval =
function(cmd, envir = globalenv())
{
 e = parse(text = cmd)
 eval(e, envir)
}
