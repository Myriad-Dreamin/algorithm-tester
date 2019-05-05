

# ifndef STD_DISTRIBUTION_DESC
# define STD_DISTRIBUTION_DESC

#include <string>

/* standard distribution as enum */
enum std_distribution {
    uniform_int_distribution,
    uniform_real_distribution,
    bernoulli_distribution,
    binomial_distribution,
    geometric_distribution,
    negative_binomial_distribution,
    poisson_distribution,
    exponential_distribution,
    gamma_distribution,
    weibull_distribution,
    extreme_value_distribution,
    normal_distribution,
    lognormal_distribution,
    chi_squared_distribution,
    cauchy_distribution,
    fisher_f_distribution,
    student_t_distribution,
    discrete_distribution,
    piecewise_constant_distribution,
    piecewise_linear_distribution
};

/* standard distribution‘s descrption as string */
const std::string distribute_desc(const std_distribution &input_distribution)
{
    switch(input_distribution)
    {
        case std_distribution::uniform_int_distribution: {
            return std::string("uniform_int_distribution");
        }
        case std_distribution::uniform_real_distribution: {
            return std::string("uniform_real_distribution");
        }
        case std_distribution::bernoulli_distribution: {
            return std::string("bernoulli_distribution");
        }
        case std_distribution::binomial_distribution: {
            return std::string("binomial_distribution");
        }
        case std_distribution::geometric_distribution: {
            return std::string("geometric_distribution");
        }
        case std_distribution::negative_binomial_distribution: {
            return std::string("negative_binomial_distribution");
        }
        case std_distribution::poisson_distribution: {
            return std::string("poisson_distribution");
        }
        case std_distribution::exponential_distribution: {
            return std::string("exponential_distribution");
        }
        case std_distribution::gamma_distribution: {
            return std::string("gamma_distribution");
        }
        case std_distribution::weibull_distribution: {
            return std::string("weibull_distribution");
        }
        case std_distribution::extreme_value_distribution: {
            return std::string("extreme_value_distribution");
        }
        case std_distribution::normal_distribution: {
            return std::string("normal_distribution");
        }
        case std_distribution::lognormal_distribution: {
            return std::string("lognormal_distribution");
        }
        case std_distribution::chi_squared_distribution: {
            return std::string("chi_squared_distribution");
        }
        case std_distribution::cauchy_distribution: {
            return std::string("cauchy_distribution");
        }
        case std_distribution::fisher_f_distribution: {
            return std::string("fisher_f_distribution");
        }
        case std_distribution::student_t_distribution: {
            return std::string("student_t_distribution");
        }
        case std_distribution::discrete_distribution: {
            return std::string("discrete_distribution");
        }
        case std_distribution::piecewise_constant_distribution: {
            return std::string("piecewise_constant_distribution");
        }
        case std_distribution::piecewise_linear_distribution: {
            return std::string("piecewise_linear_distribution");
        }
        default : {
            return std::string("unknown_distribution_type");
        }
    }
}

# endif // desc