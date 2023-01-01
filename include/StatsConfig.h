//	StatsConfig.h
//
//	Version 1.0
//	Started:	 March 5, 2001
//	Last Modified:   September 29, 2009


#ifndef __STATSCONFIG_H
#define __STATSCONFIG_H

#ifdef _DLL
#include <afx.h>
#endif
#include "linklist.h"

#if !defined(__INTEL_COMPILER) && !defined(__INTEL_LLVM_COMPILER)
#pragma managed(push, off)
#endif

#pragma pack(push, 4)

struct viewentry {						// this struct is only used for static initialization of a table of initial, or default
										// stat's entries - it could be generally used, but if you want control over which ones
										// are and are not used, then use the StatsEntry and StatsEntryCollection classes.  The
										// collection class will automatically initialize itself from an extern array of
										// viewentry's called default_entries, the list must be ended with a viewentry where
										// the attributeName is NULL
	const TCHAR	*attributeName;			// name from the tree attribute
	const TCHAR	*attributeTitle;		// name of the attribute to display
	const TCHAR	*attributeFormat;		// format string for printing the values
	const TCHAR	*attributeShortText;
	const TCHAR	*attributeTipText;
	std::int32_t operation;				// 0 = min, 1 = max, 2 = avg, 3 = total, 4 = cnt (given a range), 5 = cnt (or total) / ha, -1 is special case
	std::int32_t resolution;			// resolution of the output for floating point values
	double		range_threshold;		// if we're doing any thresholding on this entry (for counts)
	const TCHAR	*attributeRule;			// if this is set, then the threshold applies to this attribute instead of the one being reported on
	bool		defaultHidden;			// whether or not it's by default a displayable statistic
};


//exclude these classes in GCC
#if !defined(__GNUC__)
							// this class is used for generic use
class StatsEntry : public MinNode {		// for the list for all entries for the stats view
    friend class StatsEntryCollection;
    private:
	StatsEntry	*m_displaySucc;			// for the list for all entries that are selected to be displayed

    protected:
	CString		m_attributeShortTextStr,
				m_attributeNameStr,		// name of the attribute of interest
				m_attributeTitleStr,	// format string for title for display purposes
				m_attributeFormatStr,	// format string for data for display purposes
				m_attributeTipTextStr;

    public:
	std::int32_t	m_operation,
					m_resolution;
		#define OPERATION_MIN			1
		#define OPERATION_MAX			2
		#define OPERATION_MEAN			3		// min, max, average are for calculating values for floating point attributes
		#define OPERATION_MEDIAN		4
		#define OPERATION_TOTAL			5		// for counts for boolean attributes
		#define OPERATION_TOTAL_PER_HA	6		// same as OPERATION_TOTAL only per ha
		#define OPERATION_SUM			7		// sums of floating point attributes
		#define OPERATION_SUM_PER_HA	8		// same as OPERATION_SUM only per ha
		#define OPERATION_RANGE_COUNT	9		// counts for floating point attributes (whether a value fits in a range)
		#define OPERATION_UNDEFINED		0		// shouldn't be used
		#define OPERATION_DOUBLE		100
		#define OPERATION_INT			101
		#define OPERATION_STRING		102
		#define OPERATION_4CHAR			103
		#define OPERATION_BOOL			104
		#define OPERATION_DATETIME		105
		#define OPERATION_TIMESPAN		106
		#define OPERATION_STEPCOLOR		107
		#define OPERATION_STATS_FBP		108
		#define OPERATION_DOUBLEPERCENT	109
		#define OPERATION_ULONGLONG		110
		#define OPERATION_SPECIAL		-1	// to flag something that is unusual
	double		m_threshold;				// if we're doing any thresholding on this stats entry (for counts)
	CString		m_attributeRuleStr;			// if this is set, then the threshold applies to this attribute instead of the
											// one being reported on
    protected:
	StatsEntry()									{ m_displaySucc = NULL; m_operation = OPERATION_UNDEFINED; };
	StatsEntry(const StatsEntry &se)				{ *this = se; };
	StatsEntry(const TCHAR *group_name);

    public:
	virtual ~StatsEntry()							{ };
	StatsEntry &operator=(const StatsEntry &se);

	__INLINE StatsEntry *LN_Succ() const			{ return (StatsEntry *)MinNode::LN_Succ(); }
	__INLINE StatsEntry *LN_Pred() const			{ return (StatsEntry *)MinNode::LN_Pred(); };
							// iterators for the main list
	__INLINE StatsEntry *LN_Next() const			{ return m_displaySucc;	};

	void Initialize(const struct viewentry* ve);

	virtual CString m_attributeRule() const			{ if (m_attributeRuleStr.GetLength()) return m_attributeRuleStr; return ""; };
	virtual CString m_attributeName() const			{ return m_attributeNameStr; };
	virtual CString m_attributeTitle() const		{ return m_attributeTitleStr; };
	virtual CString m_attributeFormat() const		{ return m_attributeFormatStr; };
	virtual CString m_attributeShortText() const	{ return m_attributeShortTextStr; };
	virtual CString m_attributeTipText() const		{ return m_attributeTipTextStr; };

    public:			// ***** input/output...

	void SaveToIniFile(const TCHAR *group_name) const;
};


class StatsEntryCollection {
    protected:
	MinListTempl<StatsEntry>	m_statsList;	// list for all possible stat's
//			m_displayList;			// list of the stat's that are displayable - in order of display, too
	StatsEntry				*m_displayFirst;

    public:
	StatsEntryCollection();
	StatsEntryCollection(const StatsEntryCollection &sec)	{ *this = sec; };

	void Initialize(const struct viewentry *default_entries);
	void Initialize(const TCHAR *group_name);
	virtual ~StatsEntryCollection();

	StatsEntryCollection &operator=(const StatsEntryCollection &sec);

	__INLINE std::uint32_t NumStats() const					{ return m_statsList.GetCount(); };
	std::uint32_t NumDisplayableStats() const;
	bool MakeDisplayable(StatsEntry *stat, bool displayable, std::uint32_t index = (std::uint32_t)-1);
											// if (displayable) then stat is added/moved to the list at (index)
											// it's added before the displayable entry 'index', to become the new
											// 'index' entry, or if index is -1, added to the end of the list.
	std::uint32_t AddStat(StatsEntry *stat, std::uint32_t index = (std::uint32_t)-1);
											// adds a completely new stat to the set, returns its index in the main list
	void RemoveStat(StatsEntry *stat);		// removes the stat from both lists

	__INLINE std::uint32_t IndexOf(StatsEntry *stat) const	{ return m_statsList.NodeIndex(stat); };
	std::uint32_t DisplayIndexOf(StatsEntry *stat) const;

	virtual StatsEntry *New() const							{ return new StatsEntry(); };
	virtual StatsEntry *New(const StatsEntry &se) const		{ return new StatsEntry(se); };
	virtual StatsEntry *New(const TCHAR *group_name) const	{ return new StatsEntry(group_name); };

	virtual StatsEntryCollection *NewCollection()			{ return new StatsEntryCollection(); };	// RWB: really this isn't necessary but I made the default constructor
													// protected to make sure that the '=' operator is used to correctly
													// use the New() virtual functions above

	__INLINE StatsEntry *FirstStat() const					{ return m_statsList.LH_Head(); };
	__INLINE StatsEntry *StatsAt(std::uint32_t index) const	{ return m_statsList.IndexNode(index); };
	__INLINE StatsEntry *FirstDisplayStat() const			{ return m_displayFirst; };
	StatsEntry *DisplayStatsAt(std::uint32_t index) const;
	StatsEntry *FindStat(const TCHAR *attributeNameStr) const;

    public:			// ***** input/output...

	void SaveToIniFile(const TCHAR *group_name) const;
};

#endif //__GNUC__

#pragma pack(pop)

#if !defined(__INTEL_COMPILER) && !defined(__INTEL_LLVM_COMPILER)
#pragma managed(pop)
#endif

#endif /* __STATSCONFIG_H */
