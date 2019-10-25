

// This file was automatically generated by Coco/R; don't modify it.
#include "LjasParser.h"
#include "LjasErrors.h"
#include <QtDebug>
#include <QFileInfo>

namespace Ljas {


static QString coco_string_create( const wchar_t* str )
{
    return QString::fromStdWString(str);
}

int Parser::peek( quint8 la )
{
	if( la == 0 )
		return d_cur.d_type;
	else if( la == 1 )
		return d_next.d_type;
	else
		return scanner->peekToken( la - 1 ).d_type;
}


void Parser::SynErr(int n, const char* ctx) {
    if (errDist >= minErrDist)
    {
       SynErr(d_next.d_sourcePath,d_next.d_lineNr, d_next.d_colNr, n, errors, ctx);
    }
	errDist = 0;
}

void Parser::SemErr(const char* msg) {
	if (errDist >= minErrDist) errors->error(PARSER_NS::Errors::Semantics,d_cur.d_sourcePath,d_cur.d_lineNr, d_cur.d_colNr, msg);
	errDist = 0;
}

void Parser::Get() {
	for (;;) {
		d_cur = d_next;
		d_next = scanner->nextToken();
        bool deliverToParser = false;
        switch( d_next.d_type )
        {
        case PARSER_NS::Tok_Invalid:
        	if( !d_next.d_val.isEmpty() )
            	SynErr( d_next.d_type, d_next.d_val );
            // else errors already handeled in lexer
            break;
        case PARSER_NS::Tok_Comment:
            d_comments.append(d_next);
            break;
        default:
            deliverToParser = true;
            break;
        }

        if( deliverToParser )
        {
            if( d_next.d_type == PARSER_NS::Tok_Eof )
                d_next.d_type = _EOF;

            la->kind = d_next.d_type;
            if (la->kind <= maxT)
            {
                ++errDist;
                break;
            }
        }

		d_next = d_cur;
	}
}

void Parser::Expect(int n, const char* ctx ) {
	if (la->kind==n) Get(); else { SynErr(n, ctx); }
}

void Parser::ExpectWeak(int n, int follow) {
	if (la->kind == n) Get();
	else {
		SynErr(n);
		while (!StartOf(follow)) Get();
	}
}

bool Parser::WeakSeparator(int n, int syFol, int repFol) {
	if (la->kind == n) {Get(); return true;}
	else if (StartOf(repFol)) {return false;}
	else {
		SynErr(n);
		while (!(StartOf(syFol) || StartOf(repFol) || StartOf(0))) {
			Get();
		}
		return StartOf(syFol);
	}
}

void Parser::LjAsm() {
		d_stack.push(&d_root); 
		function_decl();
		d_stack.pop(); 
}

void Parser::function_decl() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_function_decl, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_function,__FUNCTION__);
		addTerminal(); 
		if (la->kind == _T_ident) {
			fname();
		}
		function_header();
		if (la->kind == _T_begin) {
			function_body();
		}
		Expect(_T_end,__FUNCTION__);
		addTerminal(); 
		if (la->kind == _T_ident) {
			fname();
		}
		d_stack.pop(); 
}

void Parser::fname() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_fname, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_ident,__FUNCTION__);
		addTerminal(); 
		d_stack.pop(); 
}

void Parser::function_header() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_function_header, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_Lpar,__FUNCTION__);
		addTerminal(); 
		formal_params();
		Expect(_T_Rpar,__FUNCTION__);
		addTerminal(); 
		if (la->kind == _T_const) {
			const_decls();
		}
		if (la->kind == _T_var) {
			var_decls();
		}
		while (la->kind == _T_function) {
			function_decl();
		}
		d_stack.pop(); 
}

void Parser::function_body() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_function_body, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_begin,__FUNCTION__);
		addTerminal(); 
		while (StartOf(1)) {
			if (la->kind == _T_ident) {
				labelDef();
			}
			statement();
		}
		d_stack.pop(); 
}

void Parser::formal_params() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_formal_params, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		while (la->kind == _T_ident) {
			vname();
		}
		d_stack.pop(); 
}

void Parser::const_decls() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_const_decls, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_const,__FUNCTION__);
		addTerminal(); 
		cname();
		Expect(_T_Eq,__FUNCTION__);
		addTerminal(); 
		const_val();
		while (la->kind == _T_ident) {
			cname();
			Expect(_T_Eq,__FUNCTION__);
			addTerminal(); 
			const_val();
		}
		d_stack.pop(); 
}

void Parser::var_decls() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_var_decls, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_var,__FUNCTION__);
		addTerminal(); 
		while (la->kind == _T_Lbrace || la->kind == _T_ident) {
			if (la->kind == _T_ident) {
				vname();
				if (la->kind == _T_Lbrack) {
					array();
				}
			} else {
				record();
			}
		}
		d_stack.pop(); 
}

void Parser::labelDef() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_labelDef, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		label();
		Expect(_T_Colon,__FUNCTION__);
		addTerminal(); 
		d_stack.pop(); 
}

void Parser::statement() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_statement, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		switch (la->kind) {
		case _T_ISLT: {
			ISLT_();
			break;
		}
		case _T_ISGE: {
			ISGE_();
			break;
		}
		case _T_ISLE: {
			ISLE_();
			break;
		}
		case _T_ISGT: {
			ISGT_();
			break;
		}
		case _T_ISEQ: {
			ISEQ_();
			break;
		}
		case _T_ISNE: {
			ISNE_();
			break;
		}
		case _T_ISTC: {
			ISTC_();
			break;
		}
		case _T_ISFC: {
			ISFC_();
			break;
		}
		case _T_IST: {
			IST_();
			break;
		}
		case _T_ISF: {
			ISF_();
			break;
		}
		case _T_MOV: {
			MOV_();
			break;
		}
		case _T_NOT: {
			NOT_();
			break;
		}
		case _T_UNM: {
			UNM_();
			break;
		}
		case _T_LEN: {
			LEN_();
			break;
		}
		case _T_ADD: {
			ADD_();
			break;
		}
		case _T_SUB: {
			SUB_();
			break;
		}
		case _T_MUL: {
			MUL_();
			break;
		}
		case _T_DIV: {
			DIV_();
			break;
		}
		case _T_MOD: {
			MOD_();
			break;
		}
		case _T_POW: {
			POW_();
			break;
		}
		case _T_CAT: {
			CAT_();
			break;
		}
		case _T_KSET: {
			KSET_();
			break;
		}
		case _T_KNIL: {
			KNIL_();
			break;
		}
		case _T_UGET: {
			UGET_();
			break;
		}
		case _T_USET: {
			USET_();
			break;
		}
		case _T_UCLO: {
			UCLO_();
			break;
		}
		case _T_FNEW: {
			FNEW_();
			break;
		}
		case _T_TNEW: {
			TNEW_();
			break;
		}
		case _T_TDUP: {
			TDUP_();
			break;
		}
		case _T_GGET: {
			GGET_();
			break;
		}
		case _T_GSET: {
			GSET_();
			break;
		}
		case _T_TGET: {
			TGET_();
			break;
		}
		case _T_TSET: {
			TSET_();
			break;
		}
		case _T_CALL: {
			CALL_();
			break;
		}
		case _T_CALLT: {
			CALLT_();
			break;
		}
		case _T_RET: {
			RET_();
			break;
		}
		case _T_FORI: {
			FORI_();
			break;
		}
		case _T_FORL: {
			FORL_();
			break;
		}
		case _T_LOOP: {
			LOOP_();
			break;
		}
		case _T_JMP: {
			JMP_();
			break;
		}
		default: SynErr(73,__FUNCTION__); break;
		}
		d_stack.pop(); 
}

void Parser::label() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_label, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_ident,__FUNCTION__);
		addTerminal(); 
		d_stack.pop(); 
}

void Parser::vname() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_vname, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_ident,__FUNCTION__);
		addTerminal(); 
		d_stack.pop(); 
}

void Parser::array() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_array, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_Lbrack,__FUNCTION__);
		addTerminal(); 
		Expect(_T_posint,__FUNCTION__);
		addTerminal(); 
		Expect(_T_Rbrack,__FUNCTION__);
		addTerminal(); 
		d_stack.pop(); 
}

void Parser::record() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_record, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_Lbrace,__FUNCTION__);
		addTerminal(); 
		vname();
		while (la->kind == _T_ident) {
			vname();
		}
		Expect(_T_Rbrace,__FUNCTION__);
		addTerminal(); 
		d_stack.pop(); 
}

void Parser::cname() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_cname, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_ident,__FUNCTION__);
		addTerminal(); 
		d_stack.pop(); 
}

void Parser::const_val() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_const_val, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		if (la->kind == _T_string) {
			Get();
			addTerminal(); 
		} else if (la->kind == _T_real || la->kind == _T_posint || la->kind == _T_negint) {
			number();
		} else if (la->kind == _T_false || la->kind == _T_nil || la->kind == _T_true) {
			primitive();
		} else if (la->kind == _T_Lbrace) {
			table_literal();
		} else SynErr(74,__FUNCTION__);
		d_stack.pop(); 
}

void Parser::number() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_number, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		if (la->kind == _T_real) {
			Get();
			addTerminal(); 
		} else if (la->kind == _T_posint || la->kind == _T_negint) {
			integer();
		} else SynErr(75,__FUNCTION__);
		d_stack.pop(); 
}

void Parser::primitive() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_primitive, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		if (la->kind == _T_nil) {
			Get();
			addTerminal(); 
		} else if (la->kind == _T_true) {
			Get();
			addTerminal(); 
		} else if (la->kind == _T_false) {
			Get();
			addTerminal(); 
		} else SynErr(76,__FUNCTION__);
		d_stack.pop(); 
}

void Parser::table_literal() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_table_literal, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_Lbrace,__FUNCTION__);
		addTerminal(); 
		while (StartOf(2)) {
			if (la->kind == _T_ident) {
				vname();
				Expect(_T_Eq,__FUNCTION__);
				addTerminal(); 
			}
			if (la->kind == _T_string) {
				Get();
				addTerminal(); 
			} else if (la->kind == _T_real || la->kind == _T_posint || la->kind == _T_negint) {
				number();
			} else if (la->kind == _T_false || la->kind == _T_nil || la->kind == _T_true) {
				primitive();
			} else SynErr(77,__FUNCTION__);
		}
		Expect(_T_Rbrace,__FUNCTION__);
		addTerminal(); 
		d_stack.pop(); 
}

void Parser::desig() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_desig, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		if (peek(1) == _T_ident && peek(2) == _T_Dot ) {
			fname();
			Expect(_T_Dot,__FUNCTION__);
			addTerminal(); 
		}
		vname();
		if (la->kind == _T_Lbrack) {
			Get();
			addTerminal(); 
			integer();
			Expect(_T_Rbrack,__FUNCTION__);
			addTerminal(); 
		}
		d_stack.pop(); 
}

void Parser::integer() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_integer, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		if (la->kind == _T_negint) {
			Get();
			addTerminal(); 
		} else if (la->kind == _T_posint) {
			Get();
			addTerminal(); 
		} else SynErr(78,__FUNCTION__);
		d_stack.pop(); 
}

void Parser::ISLT_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_ISLT_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_ISLT,__FUNCTION__);
		addTerminal(); 
		desig();
		desig();
		d_stack.pop(); 
}

void Parser::ISGE_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_ISGE_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_ISGE,__FUNCTION__);
		addTerminal(); 
		desig();
		desig();
		d_stack.pop(); 
}

void Parser::ISLE_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_ISLE_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_ISLE,__FUNCTION__);
		addTerminal(); 
		desig();
		desig();
		d_stack.pop(); 
}

void Parser::ISGT_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_ISGT_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_ISGT,__FUNCTION__);
		addTerminal(); 
		desig();
		desig();
		d_stack.pop(); 
}

void Parser::ISEQ_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_ISEQ_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_ISEQ,__FUNCTION__);
		addTerminal(); 
		desig();
		if (la->kind == _T_ident) {
			desig();
		} else if (la->kind == _T_string) {
			Get();
			addTerminal(); 
		} else if (la->kind == _T_real || la->kind == _T_posint || la->kind == _T_negint) {
			number();
		} else if (la->kind == _T_false || la->kind == _T_nil || la->kind == _T_true) {
			primitive();
		} else SynErr(79,__FUNCTION__);
		d_stack.pop(); 
}

void Parser::ISNE_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_ISNE_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_ISNE,__FUNCTION__);
		addTerminal(); 
		desig();
		if (la->kind == _T_ident) {
			desig();
		} else if (la->kind == _T_string) {
			Get();
			addTerminal(); 
		} else if (la->kind == _T_real || la->kind == _T_posint || la->kind == _T_negint) {
			number();
		} else if (la->kind == _T_false || la->kind == _T_nil || la->kind == _T_true) {
			primitive();
		} else SynErr(80,__FUNCTION__);
		d_stack.pop(); 
}

void Parser::ISTC_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_ISTC_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_ISTC,__FUNCTION__);
		addTerminal(); 
		desig();
		desig();
		d_stack.pop(); 
}

void Parser::ISFC_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_ISFC_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_ISFC,__FUNCTION__);
		addTerminal(); 
		desig();
		desig();
		d_stack.pop(); 
}

void Parser::IST_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_IST_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_IST,__FUNCTION__);
		addTerminal(); 
		desig();
		d_stack.pop(); 
}

void Parser::ISF_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_ISF_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_ISF,__FUNCTION__);
		addTerminal(); 
		desig();
		d_stack.pop(); 
}

void Parser::MOV_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_MOV_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_MOV,__FUNCTION__);
		addTerminal(); 
		desig();
		desig();
		d_stack.pop(); 
}

void Parser::NOT_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_NOT_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_NOT,__FUNCTION__);
		addTerminal(); 
		desig();
		desig();
		d_stack.pop(); 
}

void Parser::UNM_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_UNM_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_UNM,__FUNCTION__);
		addTerminal(); 
		desig();
		desig();
		d_stack.pop(); 
}

void Parser::LEN_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_LEN_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_LEN,__FUNCTION__);
		addTerminal(); 
		desig();
		desig();
		d_stack.pop(); 
}

void Parser::ADD_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_ADD_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_ADD,__FUNCTION__);
		addTerminal(); 
		desig();
		if (la->kind == _T_ident) {
			desig();
		} else if (la->kind == _T_real || la->kind == _T_posint || la->kind == _T_negint) {
			number();
		} else SynErr(81,__FUNCTION__);
		if (la->kind == _T_ident) {
			desig();
		} else if (la->kind == _T_real || la->kind == _T_posint || la->kind == _T_negint) {
			number();
		} else SynErr(82,__FUNCTION__);
		d_stack.pop(); 
}

void Parser::SUB_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_SUB_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_SUB,__FUNCTION__);
		addTerminal(); 
		desig();
		if (la->kind == _T_ident) {
			desig();
		} else if (la->kind == _T_real || la->kind == _T_posint || la->kind == _T_negint) {
			number();
		} else SynErr(83,__FUNCTION__);
		if (la->kind == _T_ident) {
			desig();
		} else if (la->kind == _T_real || la->kind == _T_posint || la->kind == _T_negint) {
			number();
		} else SynErr(84,__FUNCTION__);
		d_stack.pop(); 
}

void Parser::MUL_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_MUL_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_MUL,__FUNCTION__);
		addTerminal(); 
		desig();
		if (la->kind == _T_ident) {
			desig();
		} else if (la->kind == _T_real || la->kind == _T_posint || la->kind == _T_negint) {
			number();
		} else SynErr(85,__FUNCTION__);
		if (la->kind == _T_ident) {
			desig();
		} else if (la->kind == _T_real || la->kind == _T_posint || la->kind == _T_negint) {
			number();
		} else SynErr(86,__FUNCTION__);
		d_stack.pop(); 
}

void Parser::DIV_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_DIV_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_DIV,__FUNCTION__);
		addTerminal(); 
		desig();
		if (la->kind == _T_ident) {
			desig();
		} else if (la->kind == _T_real || la->kind == _T_posint || la->kind == _T_negint) {
			number();
		} else SynErr(87,__FUNCTION__);
		if (la->kind == _T_ident) {
			desig();
		} else if (la->kind == _T_real || la->kind == _T_posint || la->kind == _T_negint) {
			number();
		} else SynErr(88,__FUNCTION__);
		d_stack.pop(); 
}

void Parser::MOD_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_MOD_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_MOD,__FUNCTION__);
		addTerminal(); 
		desig();
		if (la->kind == _T_ident) {
			desig();
		} else if (la->kind == _T_real || la->kind == _T_posint || la->kind == _T_negint) {
			number();
		} else SynErr(89,__FUNCTION__);
		if (la->kind == _T_ident) {
			desig();
		} else if (la->kind == _T_real || la->kind == _T_posint || la->kind == _T_negint) {
			number();
		} else SynErr(90,__FUNCTION__);
		d_stack.pop(); 
}

void Parser::POW_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_POW_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_POW,__FUNCTION__);
		addTerminal(); 
		desig();
		desig();
		desig();
		d_stack.pop(); 
}

void Parser::CAT_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_CAT_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_CAT,__FUNCTION__);
		addTerminal(); 
		desig();
		desig();
		if (la->kind == _T_posint) {
			Get();
			addTerminal(); 
		}
		d_stack.pop(); 
}

void Parser::KSET_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_KSET_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_KSET,__FUNCTION__);
		addTerminal(); 
		desig();
		if (la->kind == _T_string) {
			Get();
			addTerminal(); 
		} else if (la->kind == _T_real || la->kind == _T_posint || la->kind == _T_negint) {
			number();
		} else if (la->kind == _T_false || la->kind == _T_nil || la->kind == _T_true) {
			primitive();
		} else if (la->kind == _T_ident) {
			cname();
		} else SynErr(91,__FUNCTION__);
		d_stack.pop(); 
}

void Parser::KNIL_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_KNIL_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_KNIL,__FUNCTION__);
		addTerminal(); 
		desig();
		if (la->kind == _T_posint) {
			Get();
			addTerminal(); 
		}
		d_stack.pop(); 
}

void Parser::UGET_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_UGET_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_UGET,__FUNCTION__);
		addTerminal(); 
		desig();
		desig();
		d_stack.pop(); 
}

void Parser::USET_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_USET_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_USET,__FUNCTION__);
		addTerminal(); 
		desig();
		if (la->kind == _T_string) {
			Get();
			addTerminal(); 
		} else if (la->kind == _T_real || la->kind == _T_posint || la->kind == _T_negint) {
			number();
		} else if (la->kind == _T_false || la->kind == _T_nil || la->kind == _T_true) {
			primitive();
		} else if (la->kind == _T_ident) {
			desig();
		} else SynErr(92,__FUNCTION__);
		d_stack.pop(); 
}

void Parser::UCLO_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_UCLO_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_UCLO,__FUNCTION__);
		addTerminal(); 
		desig();
		if (peek(1) == _T_ident && !( peek(2) == _T_Colon ) ) {
			label();
		}
		d_stack.pop(); 
}

void Parser::FNEW_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_FNEW_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_FNEW,__FUNCTION__);
		addTerminal(); 
		desig();
		fname();
		d_stack.pop(); 
}

void Parser::TNEW_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_TNEW_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_TNEW,__FUNCTION__);
		addTerminal(); 
		desig();
		if (la->kind == _T_posint) {
			Get();
			addTerminal(); 
			if (la->kind == _T_posint) {
				Get();
				addTerminal(); 
			}
		}
		d_stack.pop(); 
}

void Parser::TDUP_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_TDUP_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_TDUP,__FUNCTION__);
		addTerminal(); 
		desig();
		if (la->kind == _T_ident) {
			cname();
		} else if (la->kind == _T_Lbrace) {
			table_literal();
		} else SynErr(93,__FUNCTION__);
		d_stack.pop(); 
}

void Parser::GGET_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_GGET_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_GGET,__FUNCTION__);
		addTerminal(); 
		desig();
		if (la->kind == _T_string) {
			Get();
			addTerminal(); 
		} else if (la->kind == _T_ident) {
			cname();
		} else SynErr(94,__FUNCTION__);
		d_stack.pop(); 
}

void Parser::GSET_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_GSET_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_GSET,__FUNCTION__);
		addTerminal(); 
		desig();
		if (la->kind == _T_string) {
			Get();
			addTerminal(); 
		} else if (la->kind == _T_ident) {
			cname();
		} else SynErr(95,__FUNCTION__);
		d_stack.pop(); 
}

void Parser::TGET_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_TGET_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_TGET,__FUNCTION__);
		addTerminal(); 
		desig();
		desig();
		if (la->kind == _T_ident) {
			desig();
		} else if (la->kind == _T_string) {
			Get();
			addTerminal(); 
		} else if (la->kind == _T_posint) {
			Get();
			addTerminal(); 
		} else SynErr(96,__FUNCTION__);
		d_stack.pop(); 
}

void Parser::TSET_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_TSET_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_TSET,__FUNCTION__);
		addTerminal(); 
		desig();
		desig();
		if (la->kind == _T_ident) {
			desig();
		} else if (la->kind == _T_string) {
			Get();
			addTerminal(); 
		} else if (la->kind == _T_posint) {
			Get();
			addTerminal(); 
		} else SynErr(97,__FUNCTION__);
		d_stack.pop(); 
}

void Parser::CALL_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_CALL_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_CALL,__FUNCTION__);
		addTerminal(); 
		desig();
		Expect(_T_posint,__FUNCTION__);
		addTerminal(); 
		Expect(_T_posint,__FUNCTION__);
		addTerminal(); 
		d_stack.pop(); 
}

void Parser::CALLT_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_CALLT_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_CALLT,__FUNCTION__);
		addTerminal(); 
		desig();
		Expect(_T_posint,__FUNCTION__);
		addTerminal(); 
		d_stack.pop(); 
}

void Parser::RET_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_RET_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_RET,__FUNCTION__);
		addTerminal(); 
		if (peek(1) == _T_ident && ( peek(2) == _T_Dot || peek(2) == _T_ident || peek(2) == _T_Lbrack || peek(2) == _T_posint ) ) {
			desig();
			if (la->kind == _T_posint) {
				Get();
				addTerminal(); 
			}
		}
		d_stack.pop(); 
}

void Parser::FORI_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_FORI_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_FORI,__FUNCTION__);
		addTerminal(); 
		desig();
		label();
		d_stack.pop(); 
}

void Parser::FORL_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_FORL_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_FORL,__FUNCTION__);
		addTerminal(); 
		desig();
		label();
		d_stack.pop(); 
}

void Parser::LOOP_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_LOOP_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_LOOP,__FUNCTION__);
		addTerminal(); 
		d_stack.pop(); 
}

void Parser::JMP_() {
		Ljas::SynTree* n = new Ljas::SynTree( Ljas::SynTree::R_JMP_, d_next ); d_stack.top()->d_children.append(n); d_stack.push(n); 
		Expect(_T_JMP,__FUNCTION__);
		addTerminal(); 
		label();
		d_stack.pop(); 
}




// If the user declared a method Init and a mehtod Destroy they should
// be called in the contructur and the destructor respctively.
//
// The following templates are used to recognize if the user declared
// the methods Init and Destroy.

template<typename T>
struct ParserInitExistsRecognizer {
	template<typename U, void (U::*)() = &U::Init>
	struct ExistsIfInitIsDefinedMarker{};

	struct InitIsMissingType {
		char dummy1;
	};
	
	struct InitExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static InitIsMissingType is_here(...);

	// exist only if ExistsIfInitIsDefinedMarker is defined
	template<typename U>
	static InitExistsType is_here(ExistsIfInitIsDefinedMarker<U>*);

	enum { InitExists = (sizeof(is_here<T>(NULL)) == sizeof(InitExistsType)) };
};

template<typename T>
struct ParserDestroyExistsRecognizer {
	template<typename U, void (U::*)() = &U::Destroy>
	struct ExistsIfDestroyIsDefinedMarker{};

	struct DestroyIsMissingType {
		char dummy1;
	};
	
	struct DestroyExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static DestroyIsMissingType is_here(...);

	// exist only if ExistsIfDestroyIsDefinedMarker is defined
	template<typename U>
	static DestroyExistsType is_here(ExistsIfDestroyIsDefinedMarker<U>*);

	enum { DestroyExists = (sizeof(is_here<T>(NULL)) == sizeof(DestroyExistsType)) };
};

// The folloing templates are used to call the Init and Destroy methods if they exist.

// Generic case of the ParserInitCaller, gets used if the Init method is missing
template<typename T, bool = ParserInitExistsRecognizer<T>::InitExists>
struct ParserInitCaller {
	static void CallInit(T *t) {
		// nothing to do
	}
};

// True case of the ParserInitCaller, gets used if the Init method exists
template<typename T>
struct ParserInitCaller<T, true> {
	static void CallInit(T *t) {
		t->Init();
	}
};

// Generic case of the ParserDestroyCaller, gets used if the Destroy method is missing
template<typename T, bool = ParserDestroyExistsRecognizer<T>::DestroyExists>
struct ParserDestroyCaller {
	static void CallDestroy(T *t) {
		// nothing to do
	}
};

// True case of the ParserDestroyCaller, gets used if the Destroy method exists
template<typename T>
struct ParserDestroyCaller<T, true> {
	static void CallDestroy(T *t) {
		t->Destroy();
	}
};

void Parser::Parse() {
	d_cur = PARSER_NS::Token();
	d_next = PARSER_NS::Token();
	Get();
	LjAsm();
	Expect(0,__FUNCTION__);
}

Parser::Parser(PARSER_NS::Lexer *scanner, PARSER_NS::Errors* err) {
	maxT = 72;

	ParserInitCaller<Parser>::CallInit(this);
	la = &d_dummy;
	minErrDist = 2;
	errDist = minErrDist;
	this->scanner = scanner;
	errors = err;
}

bool Parser::StartOf(int s) {
	const bool T = true;
	const bool x = false;

	static bool set[3][74] = {
		{T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,T, T,T,T,T, T,T,T,T, T,T,T,T, T,T,T,T, T,T,T,T, T,T,T,T, T,T,T,T, T,T,T,T, T,T,T,T, T,T,T,x, x,x,x,x, x,x,x,x, T,x,x,x, x,x,x,x, x,x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,T,x, T,T,x,x, T,T,T,T, T,x,x,x, x,x}
	};



	return set[s][la->kind];
}

Parser::~Parser() {
	ParserDestroyCaller<Parser>::CallDestroy(this);
}

void Parser::SynErr(const QString& sourcePath, int line, int col, int n, PARSER_NS::Errors* err, const char* ctx, const QString& str ) {
	QString s;
	QString ctxStr;
	if( ctx )
		ctxStr = QString( " in %1" ).arg(ctx);
    if( n == 0 )
        s = QString("EOF expected%1").arg(ctxStr);
    else if( n < PARSER_NS::TT_Specials )
        s = QString("'%2' expected%1").arg(ctxStr).arg(PARSER_NS::tokenTypeString(n));
    else if( n <= PARSER_NS::TT_Max )
        s = QString("%2 expected%1").arg(ctxStr).arg(PARSER_NS::tokenTypeString(n));
    else
	switch (n) {
			case 0: s = coco_string_create(L"EOF expected"); break;
			case 1: s = coco_string_create(L"T_Literals_ expected"); break;
			case 2: s = coco_string_create(L"T_Lpar expected"); break;
			case 3: s = coco_string_create(L"T_Rpar expected"); break;
			case 4: s = coco_string_create(L"T_2Minus expected"); break;
			case 5: s = coco_string_create(L"T_2MinusLbrack expected"); break;
			case 6: s = coco_string_create(L"T_Dot expected"); break;
			case 7: s = coco_string_create(L"T_Colon expected"); break;
			case 8: s = coco_string_create(L"T_Eq expected"); break;
			case 9: s = coco_string_create(L"T_Lbrack expected"); break;
			case 10: s = coco_string_create(L"T_Rbrack expected"); break;
			case 11: s = coco_string_create(L"T_Rbrack2Minus expected"); break;
			case 12: s = coco_string_create(L"T_Lbrace expected"); break;
			case 13: s = coco_string_create(L"T_Rbrace expected"); break;
			case 14: s = coco_string_create(L"T_Keywords_ expected"); break;
			case 15: s = coco_string_create(L"T_ADD expected"); break;
			case 16: s = coco_string_create(L"T_CALL expected"); break;
			case 17: s = coco_string_create(L"T_CALLT expected"); break;
			case 18: s = coco_string_create(L"T_CAT expected"); break;
			case 19: s = coco_string_create(L"T_DIV expected"); break;
			case 20: s = coco_string_create(L"T_FNEW expected"); break;
			case 21: s = coco_string_create(L"T_FORI expected"); break;
			case 22: s = coco_string_create(L"T_FORL expected"); break;
			case 23: s = coco_string_create(L"T_GGET expected"); break;
			case 24: s = coco_string_create(L"T_GSET expected"); break;
			case 25: s = coco_string_create(L"T_ISEQ expected"); break;
			case 26: s = coco_string_create(L"T_ISF expected"); break;
			case 27: s = coco_string_create(L"T_ISFC expected"); break;
			case 28: s = coco_string_create(L"T_ISGE expected"); break;
			case 29: s = coco_string_create(L"T_ISGT expected"); break;
			case 30: s = coco_string_create(L"T_ISLE expected"); break;
			case 31: s = coco_string_create(L"T_ISLT expected"); break;
			case 32: s = coco_string_create(L"T_ISNE expected"); break;
			case 33: s = coco_string_create(L"T_IST expected"); break;
			case 34: s = coco_string_create(L"T_ISTC expected"); break;
			case 35: s = coco_string_create(L"T_JMP expected"); break;
			case 36: s = coco_string_create(L"T_KNIL expected"); break;
			case 37: s = coco_string_create(L"T_KSET expected"); break;
			case 38: s = coco_string_create(L"T_LEN expected"); break;
			case 39: s = coco_string_create(L"T_LOOP expected"); break;
			case 40: s = coco_string_create(L"T_MOD expected"); break;
			case 41: s = coco_string_create(L"T_MOV expected"); break;
			case 42: s = coco_string_create(L"T_MUL expected"); break;
			case 43: s = coco_string_create(L"T_NOT expected"); break;
			case 44: s = coco_string_create(L"T_POW expected"); break;
			case 45: s = coco_string_create(L"T_RET expected"); break;
			case 46: s = coco_string_create(L"T_SUB expected"); break;
			case 47: s = coco_string_create(L"T_TDUP expected"); break;
			case 48: s = coco_string_create(L"T_TGET expected"); break;
			case 49: s = coco_string_create(L"T_TNEW expected"); break;
			case 50: s = coco_string_create(L"T_TSET expected"); break;
			case 51: s = coco_string_create(L"T_UCLO expected"); break;
			case 52: s = coco_string_create(L"T_UGET expected"); break;
			case 53: s = coco_string_create(L"T_UNM expected"); break;
			case 54: s = coco_string_create(L"T_USET expected"); break;
			case 55: s = coco_string_create(L"T_begin expected"); break;
			case 56: s = coco_string_create(L"T_const expected"); break;
			case 57: s = coco_string_create(L"T_end expected"); break;
			case 58: s = coco_string_create(L"T_false expected"); break;
			case 59: s = coco_string_create(L"T_function expected"); break;
			case 60: s = coco_string_create(L"T_nil expected"); break;
			case 61: s = coco_string_create(L"T_true expected"); break;
			case 62: s = coco_string_create(L"T_var expected"); break;
			case 63: s = coco_string_create(L"T_Specials_ expected"); break;
			case 64: s = coco_string_create(L"T_ident expected"); break;
			case 65: s = coco_string_create(L"T_string expected"); break;
			case 66: s = coco_string_create(L"T_real expected"); break;
			case 67: s = coco_string_create(L"T_posint expected"); break;
			case 68: s = coco_string_create(L"T_negint expected"); break;
			case 69: s = coco_string_create(L"T_Comment expected"); break;
			case 70: s = coco_string_create(L"T_Eof expected"); break;
			case 71: s = coco_string_create(L"T_MaxToken_ expected"); break;
			case 72: s = coco_string_create(L"??? expected"); break;
			case 73: s = coco_string_create(L"invalid statement"); break;
			case 74: s = coco_string_create(L"invalid const_val"); break;
			case 75: s = coco_string_create(L"invalid number"); break;
			case 76: s = coco_string_create(L"invalid primitive"); break;
			case 77: s = coco_string_create(L"invalid table_literal"); break;
			case 78: s = coco_string_create(L"invalid integer"); break;
			case 79: s = coco_string_create(L"invalid ISEQ_"); break;
			case 80: s = coco_string_create(L"invalid ISNE_"); break;
			case 81: s = coco_string_create(L"invalid ADD_"); break;
			case 82: s = coco_string_create(L"invalid ADD_"); break;
			case 83: s = coco_string_create(L"invalid SUB_"); break;
			case 84: s = coco_string_create(L"invalid SUB_"); break;
			case 85: s = coco_string_create(L"invalid MUL_"); break;
			case 86: s = coco_string_create(L"invalid MUL_"); break;
			case 87: s = coco_string_create(L"invalid DIV_"); break;
			case 88: s = coco_string_create(L"invalid DIV_"); break;
			case 89: s = coco_string_create(L"invalid MOD_"); break;
			case 90: s = coco_string_create(L"invalid MOD_"); break;
			case 91: s = coco_string_create(L"invalid KSET_"); break;
			case 92: s = coco_string_create(L"invalid USET_"); break;
			case 93: s = coco_string_create(L"invalid TDUP_"); break;
			case 94: s = coco_string_create(L"invalid GGET_"); break;
			case 95: s = coco_string_create(L"invalid GSET_"); break;
			case 96: s = coco_string_create(L"invalid TGET_"); break;
			case 97: s = coco_string_create(L"invalid TSET_"); break;

		default:
		{
			s = QString( "generic error %1").arg(n);
		}
		break;
	}
    if( !str.isEmpty() )
        s = QString("%1 %2").arg(s).arg(str);
	if( err )
		err->error(PARSER_NS::Errors::Syntax, sourcePath, line, col, s);
	else
		qCritical() << "Error Parser" << line << col << s;
	//count++;
}

} // namespace

