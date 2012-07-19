/**
 * @file matrix_ewise_eval.h
 *
 * Evaluation of element-wise matrix expressions
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHTMAT_MATRIX_EWISE_EVAL_H_
#define LIGHTMAT_MATRIX_EWISE_EVAL_H_

#include <light_mat/matrix/matrix_ewise_expr.h>
#include <light_mat/matrix/generic_matrix_eval.h>

namespace lmat
{

	/********************************************
	 *
	 *  Vector evaluator classes
	 *
	 ********************************************/

	template<class Fun, class Arg>
	class unary_ewise_linear_evaluator
	: public ILinearVectorEvaluator<
	  	  unary_ewise_linear_evaluator<Fun, Arg>,
	  	  typename Fun::result_type>
	{
		typedef typename unwrapped_expr<Arg>::type arg_expr_t;

	public:
		unary_ewise_linear_evaluator(const unary_ewise_expr<Fun, Arg>& expr)
		: m_fun(expr.fun()), m_arg_eval(expr.arg())
		{
		}

		LMAT_ENSURE_INLINE typename Fun::result_type get_value(const index_t i) const
		{
			return m_fun(m_arg_eval.get_value(i));
		}

	private:
		Fun m_fun;
		typedef typename linear_eval<arg_expr_t>::evaluator_type arg_eval_t;
		arg_eval_t m_arg_eval;
	};

	template<class Fun, class Arg>
	class unary_ewise_percol_evaluator
	: public IPerColVectorEvaluator<
	  	  unary_ewise_percol_evaluator<Fun, Arg>,
	  	  typename Fun::result_type>
	{
		typedef typename unwrapped_expr<Arg>::type arg_expr_t;

	public:
		unary_ewise_percol_evaluator(const unary_ewise_expr<Fun, Arg>& expr)
		: m_fun(expr.fun()), m_arg_eval(expr.arg())
		{
		}

		LMAT_ENSURE_INLINE typename Fun::result_type get_value(const index_t i) const
		{
			return m_fun(m_arg_eval.get_value(i));
		}

		LMAT_ENSURE_INLINE void next_column()
		{
			m_arg_eval.next_column();
		}

	private:
		Fun m_fun;
		typedef typename percol_eval<arg_expr_t>::evaluator_type arg_eval_t;
		arg_eval_t m_arg_eval;
	};


	template<class Fun, class Arg1, class Arg2>
	class binary_ewise_linear_evaluator
	: public ILinearVectorEvaluator<
	  	  binary_ewise_linear_evaluator<Fun, Arg1, Arg2>,
	  	  typename Fun::result_type>
	{
		typedef typename unwrapped_expr<Arg1>::type arg1_expr_t;
		typedef typename unwrapped_expr<Arg2>::type arg2_expr_t;

	public:
		binary_ewise_linear_evaluator(const binary_ewise_expr<Fun, Arg1, Arg2>& expr)
		: m_fun(expr.fun()), m_arg1_eval(expr.first_arg()), m_arg2_eval(expr.second_arg())
		{
		}

		LMAT_ENSURE_INLINE typename Fun::result_type get_value(const index_t i) const
		{
			return m_fun(m_arg1_eval.get_value(i), m_arg2_eval.get_value(i));
		}

	private:
		Fun m_fun;
		typedef typename linear_eval<arg1_expr_t>::evaluator_type arg1_eval_t;
		typedef typename linear_eval<arg2_expr_t>::evaluator_type arg2_eval_t;

		arg1_eval_t m_arg1_eval;
		arg2_eval_t m_arg2_eval;
	};


	template<class Fun, class Arg1, class Arg2>
	class binary_ewise_percol_evaluator
	: public IPerColVectorEvaluator<
	  	  binary_ewise_percol_evaluator<Fun, Arg1, Arg2>,
	  	  typename Fun::result_type>
	{
		typedef typename unwrapped_expr<Arg1>::type arg1_expr_t;
		typedef typename unwrapped_expr<Arg2>::type arg2_expr_t;

	public:
		binary_ewise_percol_evaluator(const binary_ewise_expr<Fun, Arg1, Arg2>& expr)
		: m_fun(expr.fun()), m_arg1_eval(expr.first_arg()), m_arg2_eval(expr.second_arg())
		{
		}

		LMAT_ENSURE_INLINE typename Fun::result_type get_value(const index_t i) const
		{
			return m_fun(m_arg1_eval.get_value(i), m_arg2_eval.get_value(i));
		}

		LMAT_ENSURE_INLINE void next_column()
		{
			m_arg1_eval.next_column();
			m_arg2_eval.next_column();
		}

	private:
		Fun m_fun;
		typedef typename percol_eval<arg1_expr_t>::evaluator_type arg1_eval_t;
		typedef typename percol_eval<arg2_expr_t>::evaluator_type arg2_eval_t;

		arg1_eval_t m_arg1_eval;
		arg2_eval_t m_arg2_eval;
	};



	/********************************************
	 *
	 *  Evaluator dispatch
	 *
	 ********************************************/

	template<typename Fun, class Arg>
	struct linear_eval<unary_ewise_expr<Fun, Arg> >
	{
		typedef unary_ewise_linear_evaluator<Fun, Arg> evaluator_type;

		LMAT_ENSURE_INLINE
		static int cost_of(const unary_ewise_expr<Fun, Arg>& expr )
		{
			return linear_eval<typename unwrapped_expr<Arg>::type>::cost_of(expr.arg());
		}
	};

	template<typename Fun, class Arg>
	struct percol_eval<unary_ewise_expr<Fun, Arg> >
	{
		typedef unary_ewise_percol_evaluator<Fun, Arg> evaluator_type;

		LMAT_ENSURE_INLINE
		static int cost_of(const unary_ewise_expr<Fun, Arg>& expr )
		{
			return percol_eval<typename unwrapped_expr<Arg>::type>::cost_of(expr.arg());
		}
	};

	template<typename Fun, class Arg1, class Arg2>
	struct linear_eval<binary_ewise_expr<Fun, Arg1, Arg2> >
	{
		typedef binary_ewise_linear_evaluator<Fun, Arg1, Arg2> evaluator_type;

		LMAT_ENSURE_INLINE
		static int cost_of(const binary_ewise_expr<Fun, Arg1, Arg2>& expr )
		{
			return  linear_eval<typename unwrapped_expr<Arg1>::type>::cost_of(expr.first_arg()) +
					linear_eval<typename unwrapped_expr<Arg2>::type>::cost_of(expr.second_arg());
		}
	};

	template<typename Fun, class Arg1, class Arg2>
	struct percol_eval<binary_ewise_expr<Fun, Arg1, Arg2> >
	{
		typedef binary_ewise_percol_evaluator<Fun, Arg1, Arg2> evaluator_type;

		LMAT_ENSURE_INLINE
		static int cost_of(const binary_ewise_expr<Fun, Arg1, Arg2>& expr )
		{
			return  percol_eval<typename unwrapped_expr<Arg1>::type>::cost_of(expr.first_arg()) +
					percol_eval<typename unwrapped_expr<Arg2>::type>::cost_of(expr.second_arg());
		}
	};

}

#endif /* MATRIX_EWISE_EVAL_H_ */
