

// This file was automatically generated by Coco/R; don't modify it.
#if !defined(Ljas_COCO_PARSER_H__)
#define Ljas_COCO_PARSER_H__

#include <QStack>
#include <LjTools/LjasSynTree.h>


#define PARSER_NS Ljas
#include <LjTools/LjasLexer.h>

namespace Ljas {



class Parser {
private:
	enum {
		_EOF=0,
		_T_Literals_=1,
		_T_Lpar=2,
		_T_Rpar=3,
		_T_2Minus=4,
		_T_2MinusLbrack=5,
		_T_Dot=6,
		_T_Colon=7,
		_T_Eq=8,
		_T_Lbrack=9,
		_T_Rbrack=10,
		_T_Rbrack2Minus=11,
		_T_Lbrace=12,
		_T_Rbrace=13,
		_T_Keywords_=14,
		_T_ADD=15,
		_T_CALL=16,
		_T_CALLT=17,
		_T_CAT=18,
		_T_DIV=19,
		_T_FNEW=20,
		_T_FORI=21,
		_T_FORL=22,
		_T_GGET=23,
		_T_GSET=24,
		_T_ISEQ=25,
		_T_ISF=26,
		_T_ISFC=27,
		_T_ISGE=28,
		_T_ISGT=29,
		_T_ISLE=30,
		_T_ISLT=31,
		_T_ISNE=32,
		_T_IST=33,
		_T_ISTC=34,
		_T_JMP=35,
		_T_KNIL=36,
		_T_KSET=37,
		_T_LEN=38,
		_T_LOOP=39,
		_T_MOD=40,
		_T_MOV=41,
		_T_MUL=42,
		_T_NOT=43,
		_T_POW=44,
		_T_RET=45,
		_T_SUB=46,
		_T_TDUP=47,
		_T_TGET=48,
		_T_TNEW=49,
		_T_TSET=50,
		_T_UCLO=51,
		_T_UGET=52,
		_T_UNM=53,
		_T_USET=54,
		_T_begin=55,
		_T_const=56,
		_T_end=57,
		_T_false=58,
		_T_function=59,
		_T_nil=60,
		_T_true=61,
		_T_var=62,
		_T_Specials_=63,
		_T_ident=64,
		_T_string=65,
		_T_real=66,
		_T_posint=67,
		_T_negint=68,
		_T_Comment=69,
		_T_Eof=70,
		_T_MaxToken_=71
	};
	int maxT;

	int errDist;
	int minErrDist;

	void SynErr(int n, const char* ctx = 0);
	void Get();
	void Expect(int n, const char* ctx = 0);
	bool StartOf(int s);
	void ExpectWeak(int n, int follow);
	bool WeakSeparator(int n, int syFol, int repFol);
    void SynErr(const QString& sourcePath, int line, int col, int n, PARSER_NS::Errors* err, const char* ctx, const QString& = QString() );

public:
	PARSER_NS::Lexer *scanner;
	PARSER_NS::Errors  *errors;

	PARSER_NS::Token d_cur;
	PARSER_NS::Token d_next;
	QList<PARSER_NS::Token> d_comments;
	struct TokDummy
	{
		int kind;
	};
	TokDummy d_dummy;
	TokDummy *la;			// lookahead token
	
	int peek( quint8 la = 1 );

    void RunParser()
    {
        d_stack.push(&d_root);
        Parse();
        d_stack.pop();
    }
        
Ljas::SynTree d_root;
	QStack<Ljas::SynTree*> d_stack;
	void addTerminal() {
		Ljas::SynTree* n = new Ljas::SynTree( d_cur ); d_stack.top()->d_children.append(n);
	}



	Parser(PARSER_NS::Lexer *scanner,PARSER_NS::Errors*);
	~Parser();
	void SemErr(const char* msg);

	void LjAsm();
	void function_decl();
	void fname();
	void function_header();
	void function_body();
	void formal_params();
	void const_decls();
	void var_decls();
	void labelDef();
	void statement();
	void label();
	void vname();
	void array();
	void record();
	void cname();
	void const_val();
	void number();
	void primitive();
	void table_literal();
	void desig();
	void integer();
	void ISLT_();
	void ISGE_();
	void ISLE_();
	void ISGT_();
	void ISEQ_();
	void ISNE_();
	void ISTC_();
	void ISFC_();
	void IST_();
	void ISF_();
	void MOV_();
	void NOT_();
	void UNM_();
	void LEN_();
	void ADD_();
	void SUB_();
	void MUL_();
	void DIV_();
	void MOD_();
	void POW_();
	void CAT_();
	void KSET_();
	void KNIL_();
	void UGET_();
	void USET_();
	void UCLO_();
	void FNEW_();
	void TNEW_();
	void TDUP_();
	void GGET_();
	void GSET_();
	void TGET_();
	void TSET_();
	void CALL_();
	void CALLT_();
	void RET_();
	void FORI_();
	void FORL_();
	void LOOP_();
	void JMP_();

	void Parse();

}; // end Parser

} // namespace


#endif

