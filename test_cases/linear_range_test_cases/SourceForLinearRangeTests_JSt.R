findGaussData <- function(numberofplayers, quota) {
  #
  # With this function you can regenerate all input files
  # for the range tests ...
  #
  # J.St. --- March 4, 2021
  ourAverage <- quota / numberofplayers
  # make numberofplayers / 2 pairs, where pair_x + pair_y <= 4*ourAverage
  # increment -> ourAverage / (numberofplayers / 4) -> increment used
  # Adjust first weight such that weight sum = 2*quota-1
  increment <- ourAverage / (numberofplayers / 4)
  i <- 0
  numbervector <- c()
  ourSum <- 0
  numbervector <- rep(0,numberofplayers)
  while (i < numberofplayers) {
    number1 <- 4 * ourAverage - ((i / 2) * increment) - 1 
    number2 <- increment * (i / 2) + 1
    print(as.integer(number1))
    print(as.integer(number2))
    ourSum <- ourSum + as.integer(number1) + as.integer(number2)
    numbervector[i+1] = as.integer(number1)
    numbervector[i+2] = as.integer(number2)
    i = i + 2
  }
  numbervector[1] = numbervector[1] + ((2*quota - 1) - ourSum)
  print(ourSum)
  print(sum(numbervector))
  print(increment)
  print(ourAverage)
  return(numbervector)
}