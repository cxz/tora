//***************************************************************************
/*
 * TOra - An Oracle Toolkit for DBA's and developers
 * Copyright (C) 2000 GlobeCom AB
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *      As a special exception, you have permission to link this program
 *      with the Oracle Client libraries and distribute executables, as long
 *      as you follow the requirements of the GNU GPL in regard to all of the
 *      software in the executable aside from Oracle client libraries.
 *
 *      Specifically you are not permitted to link this program with the
 *      Qt/UNIX or Qt/Windows products of TrollTech. And you are not
 *      permitted to distribute binaries compiled against these libraries
 *      without written consent from GlobeCom AB. Observe that this does not
 *      disallow linking to the Qt Free Edition.
 *
 * All trademarks belong to their respective owners.
 *
 ****************************************************************************/

#ifndef __TO_EXTRACT
#define __TO_EXTRACT

#include "toconnection.h"

// Liberally ported from DDL::Oracle 1.06
// Copyright (c) 2000, 2001 Richard Sutherland - United States of America

class toExtractProgress;

/**
 * This class can be used to reverse engineered database objects.
 */

class toExtract {
  toConnection &Connection;
  QWidget *Parent;

  // Attributes
  QString Schema;
  QString Resize;
  bool Code;
  bool Comments;
  bool Constraints;
  bool Contents;
  bool Grants;
  bool Heading;
  bool Indexes;
  bool Parallel;
  bool Partition;
  bool Prompt;
  bool Storage;

  // Flags
  bool IsASnapIndex;
  bool IsASnapTable;
  bool Describe;

  // Database info
  int BlockSize;
  list<QString> Initial;
  list<QString> Next;
  list<QString> Limit;

  // Compile functions
  QString compile(const QString &schema,const QString &owner,const QString &name,
		  const QString &type);
  QString compilePackage(const QString &schema,const QString &owner,const QString &name,
			 const QString &type);

  // Create functions
  QString createConstraint(const QString &schema,const QString &owner,const QString &name);
  QString createDBLink(const QString &schema,const QString &owner,const QString &name);
  QString createExchangeIndex(const QString &schema,const QString &owner,const QString &name);
  QString createExchangeTable(const QString &schema,const QString &owner,const QString &name);
  QString createFunction(const QString &schema,const QString &owner,const QString &name);
  QString createIndex(const QString &schema,const QString &owner,const QString &name);
  QString createMaterializedView(const QString &schema,const QString &owner,const QString &name);
  QString createMaterializedViewLog(const QString &schema,const QString &owner,const QString &name);
  QString createPackage(const QString &schema,const QString &owner,const QString &name);
  QString createPackageBody(const QString &schema,const QString &owner,const QString &name);
  QString createProcedure(const QString &schema,const QString &owner,const QString &name);
  QString createProfile(const QString &schema,const QString &owner,const QString &name);
  QString createRole(const QString &schema,const QString &owner,const QString &name);
  QString createRollbackSegment(const QString &schema,const QString &owner,const QString &name);
  QString createSequence(const QString &schema,const QString &owner,const QString &name);
  QString createSnapshot(const QString &schema,const QString &owner,const QString &name);
  QString createSnapshotLog(const QString &schema,const QString &owner,const QString &name);
  QString createSynonym(const QString &schema,const QString &owner,const QString &name);
  QString createTable(const QString &schema,const QString &owner,const QString &name);
  QString createTableFamily(const QString &schema,const QString &owner,const QString &name);
  QString createTableContents(const QString &schema,const QString &owner,const QString &name);
  QString createTableReferences(const QString &schema,const QString &owner,const QString &name);
  QString createTablespace(const QString &schema,const QString &owner,const QString &name);
  QString createTrigger(const QString &schema,const QString &owner,const QString &name);
  QString createType(const QString &schema,const QString &owner,const QString &name);
  QString createUser(const QString &schema,const QString &owner,const QString &name);
  QString createView(const QString &schema,const QString &owner,const QString &name);

  // Description functions
  void describeConstraint(list<QString> &lst,const QString &schema,const QString &owner,
			  const QString &name);
  void describeDBLink(list<QString> &lst,const QString &schema,const QString &owner,
		      const QString &name);
  void describeExchangeIndex(list<QString> &lst,const QString &schema,const QString &owner,
			     const QString &name);
  void describeExchangeTable(list<QString> &lst,const QString &schema,const QString &owner,
			     const QString &name);
  void describeFunction(list<QString> &lst,const QString &schema,const QString &owner,
			const QString &name);
  void describeIndex(list<QString> &lst,const QString &schema,const QString &owner,
		     const QString &name);
  void describeMaterializedView(list<QString> &lst,const QString &schema,const QString &owner,
				const QString &name);
  void describeMaterializedViewLog(list<QString> &lst,const QString &schema,const QString &owner,
				   const QString &name);
  void describePackage(list<QString> &lst,const QString &schema,const QString &owner,
		       const QString &name);
  void describePackageBody(list<QString> &lst,const QString &schema,const QString &owner,
			   const QString &name);
  void describeProcedure(list<QString> &lst,const QString &schema,const QString &owner,
			 const QString &name);
  void describeProfile(list<QString> &lst,const QString &schema,const QString &owner,
		       const QString &name);
  void describeRole(list<QString> &lst,const QString &schema,const QString &owner,
		    const QString &name);
  void describeRollbackSegment(list<QString> &lst,const QString &schema,const QString &owner,
			       const QString &name);
  void describeSequence(list<QString> &lst,const QString &schema,const QString &owner,
			const QString &name);
  void describeSnapshot(list<QString> &lst,const QString &schema,const QString &owner,
			const QString &name);
  void describeSnapshotLog(list<QString> &lst,const QString &schema,const QString &owner,
			   const QString &name);
  void describeSynonym(list<QString> &lst,const QString &schema,const QString &owner,
		       const QString &name);
  void describeTable(list<QString> &lst,const QString &schema,const QString &owner,
		     const QString &name);
  void describeTableFamily(list<QString> &lst,const QString &schema,const QString &owner,
			   const QString &name);
  void describeTableReferences(list<QString> &lst,const QString &schema,const QString &owner,
			       const QString &name);
  void describeTablespace(list<QString> &lst,const QString &schema,const QString &owner,
			  const QString &name);
  void describeTrigger(list<QString> &lst,const QString &schema,const QString &owner,
		       const QString &name);
  void describeType(list<QString> &lst,const QString &schema,const QString &owner,
		    const QString &name);
  void describeUser(list<QString> &lst,const QString &schema,const QString &owner,
		    const QString &name);
  void describeView(list<QString> &lst,const QString &schema,const QString &owner,
		    const QString &name);

  // Drop functions
  QString dropConstraint(const QString &schema,const QString &name,const QString &type,
			 const QString &owner);
  QString dropDatabaseLink(const QString &schema,const QString &name,const QString &type,
			   const QString &owner);
  QString dropMViewLog(const QString &schema,const QString &name,const QString &type,
		       const QString &owner);
  QString dropObject(const QString &schema,const QString &name,const QString &type,
		     const QString &owner);
  QString dropProfile(const QString &schema,const QString &name,const QString &type,
		      const QString &owner);
  QString dropSchemaObject(const QString &schema,const QString &name,const QString &type,
			   const QString &owner);
  QString dropSynonym(const QString &schema,const QString &name,const QString &type,
		      const QString &owner);
  QString dropTable(const QString &schema,const QString &name,const QString &type,
		    const QString &owner);
  QString dropTablespace(const QString &schema,const QString &name,const QString &type,
			 const QString &owner);
  QString dropUser(const QString &schema,const QString &name,const QString &type,
		   const QString &owner);

  // Resize functions
  QString resizeIndex(const QString &schema,const QString &owner,const QString &name);
  QString resizeTable(const QString &schema,const QString &owner,const QString &name);

  // Misc extract functions
  QString constraintColumns(const QString &owner,const QString &name);
  QString createComments(const QString &schema,const QString &owner,const QString &name);
  QString createIOT(const QString &schema,const QString &owner,const QString &name);
  QString createMView(const QString &schema,const QString &owner,const QString &name,
		      const QString &type);
  QString createMViewIndex(const QString &schema,const QString &owner,const QString &name);
  QString createMViewLog(const QString &schema,const QString &owner,const QString &name,
			 const QString &type);
  QString createMViewTable(const QString &schema,const QString &owner,const QString &name);
  QString createPartitionedIOT(const QString &schema,const QString &owner,const QString &name);
  QString createPartitionedIndex(const QString &schema,const QString &owner,const QString &name,
				 const QString &soFar);
  QString createPartitionedTable(const QString &schema,const QString &owner,const QString &name);
  QString createTableText(list<QString> &result,const QString &schema,const QString &owner,const QString &name);
  QString displaySource(const QString &schema,const QString &owner,const QString &name,
			const QString &type);
  QString grantedPrivs(const QString &schema,const QString &name,int type=3);
  QString indexColumns(const QString &schema,const QString &owner,const QString &name);
  QString keyColumns(const QString &owner,const QString &name,const QString &type,
		     const QString &table);
  QString partitionKeyColumns(const QString &owner,const QString &name,const QString &type);
  QString rangePartitions(const QString &owner,const QString &name,
			  const QString &subPartitionType,const QString &locality);
  QString resizeIndexPartition(const QString &schema,const QString &owner,const QString &name,
			       const QString &partition,const QString &seqType);
  QString resizeTablePartition(const QString &schema,const QString &owner,const QString &name,
			       const QString &partition,const QString &seqType);
  QString subPartitionKeyColumns(const QString &owner,const QString &name,const QString &type);
  QString tableColumns(const QString &owner,const QString &name);

  // Misc describe functions
  void describeAttributes(list<QString> &lst,list<QString> &ctx,list<QString> &result);
  void describeComments(list<QString> &lst,list<QString> &ctx,
			const QString &schema,const QString &owner,const QString &name);
  void describePrivs(list<QString> &lst,list<QString> &ctx,const QString &name);
  void describeIndexColumns(list<QString> &lst,list<QString> &ctx,
			    const QString &owner,const QString &name);
  void describeIOT(list<QString> &lst,list<QString> &ctx,
		   const QString &schema,const QString &owner,const QString &name);
  void describeMView(list<QString> &lst,const QString &schema,
		     const QString &owner,const QString &name,const QString &type);
  void describeMViewIndex(list<QString> &lst,list<QString> &ctx,const QString &schema,
			  const QString &owner,const QString &name);
  void describeMViewLog(list<QString> &lst,const QString &schema,
			const QString &owner,const QString &name,const QString &type);
  void describeMViewTable(list<QString> &lst,list<QString> &ctx,const QString &schema,
			  const QString &owner,const QString &name);
  void describePartitionedIndex(list<QString> &lst,list<QString> &ctx,
				const QString &schema,const QString &owner,const QString &name);
  void describePartitionedIOT(list<QString> &lst,list<QString> &ctx,
			      const QString &schema,const QString &owner,const QString &name);
  void describePartitionedTable(list<QString> &lst,list<QString> &ctx,
				const QString &schema,const QString &owner,const QString &name);
  void describePartitions(list<QString> &lst,list<QString> &ctx,
			  const QString &owner,const QString &name,
			  const QString &subPartitionType,const QString &locality);
  void describeTableText(list<QString> &lst,list<QString> &ctx,list<QString> &result,
			 const QString &schema,const QString &owner,const QString &name);
  void describeTableColumns(list<QString> &lst,list<QString> &ctx,
			    const QString &owner,const QString &name);
  void describeSource(list<QString> &lst,const QString &schema,const QString &owner,
		      const QString &name,const QString &type);

  // Misc functions
  void addDescription(list<QString> &ret,list<QString> &ctx,
		      const QString &arg1=QString::null,const QString &arg2=QString::null,
		      const QString &arg3=QString::null,const QString &arg4=QString::null,
		      const QString &arg5=QString::null,const QString &arg6=QString::null,
		      const QString &arg7=QString::null,const QString &arg8=QString::null,
		      const QString &arg9=QString::null);
  void clearFlags(void);
  QString generateHeading(const QString &action,list<QString> &list);
  void initialNext(const QString &blocks,QString &initial,QString &next);
  void objectExists(const QString &owner,const QString &name,const QString &type);
  QString prepareDB(const QString &data);
  QString segmentAttributes(list<QString> &result);
  QString intSchema(const QString &owner);
  void setSizes(void);
  QString reContext(list<QString> &ctx,int strip,const QString &str);
  void rethrow(const QString &what,const QString &object,const otl_exception &exc);
public:
  /** Create a new extractor.
   * @param conn Connection to extract from.
   * @param parent Parent widget of progress indicator.
   */

  toExtract(toConnection &conn,QWidget *parent);

  /** Compile the following list of objects.
   * @param object List of object. This has the format {type}:{schema}.{object}.
   *               The type can be any of FUNCTION, PROCEDURE, TRIGGER, VIEW or PACKAGE.
   * @return A string containing a script to compile the specified objects.
   */
  QString compile(list<QString> &object);

  /** Create script to recreate list of objects.
   * @param object List of object. This has the format {type}:{schema}.{object}.
   *               The type can be any of CONSTRAINT, DATABASE LINK, EXCHANGE INDEX,
   *               EXCHANGE TABLE, FUNCTION, INDEX, MATERIALIZED VIEW,
   *               MATERIALIZED VIEW LOG, PACKAGE, PACKAGE BODY, PROCEDURE,
   *               PROFILE, ROLE, ROLE GRANTS, ROLLBACK SEGMENT, SEQUENCE,
   *               SNAPSHOT, SNAPSHOT LOG, SYNONYM, TABLE, TABLE FAMILY,
   *               TABLE CONTENTS, TABLE REFERENCES, TABLESPACE, TRIGGER,
   *               TRIGGER, TYPE, USER, USER GRANTS.
   * @return A string containing a script to recreate the specified objects.
   */
  QString create(list<QString> &object);

  /** Create a description of objects.
   * @param object List of object. This has the format {type}:{schema}.{object}.
   *               The type can be any of CONSTRAINT, DATABASE LINK, EXCHANGE INDEX,
   *               EXCHANGE TABLE, FUNCTION, INDEX, MATERIALIZED VIEW,
   *               MATERIALIZED VIEW LOG, PACKAGE, PACKAGE BODY, PROCEDURE,
   *               PROFILE, ROLE, ROLE GRANTS, ROLLBACK SEGMENT, SEQUENCE,
   *               SNAPSHOT, SNAPSHOT LOG, SYNONYM, TABLE, TABLE FAMILY,
   *               TABLE CONTENTS, TABLE REFERENCES, TABLESPACE, TRIGGER,
   *               TRIGGER, TYPE, USER, USER GRANTS.
   * @return A list of strings describing the objects. Each string should be
   *         considered like a list of strings separated by the character '\001'.
   *         The later in each string the smaller item the change and it is hierachical.
   */
  list<QString> describe(list<QString> &object);

  /** Create script to drop a list of objects.
   * @param object List of object. This has the format {type}:{schema}.{object}.
   *               The type can be any of CONSTRAINT, DATABASE LINK, DIMENSION
   *               DIRECTORY, FUNCTION, INDEX, MATERIALIZED VIEW,
   *               MATERIALIZED VIEW LOG, PACKAGE, PROCEDURE,
   *               PROFILE, ROLE, ROLLBACK SEGMENT, SEQUENCE,
   *               SNAPSHOT, SNAPSHOT LOG, SYNONYM, TABLE, TABLESPACE,
   *               TRIGGER, TYPE, USER.
   * @return A string containing a script to drop the specified objects.
   */
  QString drop(list<QString> &object);
  /** Not implemented yet.
   */
  QString resize(list<QString> &object);
  /** Not implemented yet.
   */
  QString migrate(list<QString> &drpLst,list<QString> &crtLst);

  /** Parse an object string to get owner and name of the object.
   * @param object Object string on the format {owner}.{name}.
   * @param owner Reference to string which will get the object owner.
   * @param name Reference to string which will get the object name.
   */
  void parseObject(const QString &object,QString &owner,QString &name);

  /** Set the schema of the extraction.
   * @param schema Specify the schema of the output script or description. If empty
   *               don't specify any object. If the string "1" use same object as input.
   *               Otherwise use the specified schema.
   */
  void setSchema(const QString &schema)
  { Schema=schema; }
  /** Set sizes to use for resize of object on extraction.
   * @param A list of sizes separated by ':'. Should be an even multiple of three where
   *        the first value is the largest current size to use these values. The next
   *        value is the initial value to use, the last is the next increment value to
   *        use. As a special case the string "1" can be used to set up auto resize.
   */
  void setResize(const QString &resize)
  { Resize=resize; setSizes(); }
  /** Set inclusion of prompts.
   * @param prompt If prompt generation should be generated.
   */
  void setPrompt(bool prompt)
  { Prompt=prompt; }
  /** Include heading in scripts.
   * @param heading Include heading in scripts.
   */
  void setHeading(bool heading)
  { Heading=heading; }
  /** Include constraints in extraction.
   * @param constraints Include constraints.
   */
  void setConstraints(bool constraints)
  { Constraints=constraints; }
  /** Include indexes in extraction.
   * @param indexes Include indexes.
   */
  void setIndexes(bool indexes)
  { Indexes=indexes; }
  /** Include grants in extraction.
   * @param val Include grants.
   */
  void setGrants(bool val)
  { Grants=val; }
  /** Include storage specification in extraction.
   * @param val Include storage specification.
   */
  void setStorage(bool val)
  { Storage=val; }
  /** Include parallel specification in extraction.
   * @param val Include parallel specification.
   */
  void setParallel(bool val)
  { Parallel=val; }
  /** Include partition specification in extraction.
   * @param val Include partition specification.
   */
  void setPartition(bool val)
  { Partition=val; }
  /** Include contents of tables in scripts.
   * @param val Include contents of tables.
   */
  void setContents(bool val)
  { Contents=val; }
  /** Include comments in extraction.
   * @param val Include indexes.
   */
  void setComments(bool val)
  { Comments=val; }
  /** Include code in extraction.
   * @param val Include code.
   */
  void setCode(bool val)
  { Code=val; }

  /** Get schema specification.
   * @return Schema specification.
   * @see setSchema
   */
  const QString &getSchema(void)
  { return Schema; }
  /** Get resize specification.
   * @return Resize specification.
   * @see setResize
   */
  bool getResize(void)
  { return Resize; }
  /** Check if prompt are generated.
   * @return If prompts are generated.
   */
  bool getPrompt(void)
  { return Prompt; }
  /** Check if headings are generated.
   * @return If headings are generated.
   */
  bool getHeading(void)
  { return Heading; }
  /** Check if constraints are generated.
   * @return If constraints are generated.
   */
  bool getConstraints(void)
  { return Constraints; }
  /** Check if indexes are generated.
   * @return If indexes are generated.
   */
  bool getIndexes(void)
  { return Indexes; }
  /** Check if grants are generated.
   * @return If grants are generated.
   */
  bool getGrants(void)
  { return Grants; }
  /** Check if storage specification are generated.
   * @return If storage specification are generated.
   */
  bool getStorage(void)
  { return Storage; }
  /** Check if parallell specification are generated.
   * @return If parallell specification are generated.
   */
  bool getParallel(void)
  { return Parallel; }
  /** Check if partition specification are generated.
   * @return If partition specification  are generated.
   */
  bool getPartition(void)
  { return Partition; }
  /** Check if contents of tables are generated.
   * @return If content is generated.
   */
  bool getContents(void)
  { return Contents; }
  /** Check if comments are generated.
   * @return If comments are generated.
   */
  bool getComments(void)
  { return Comments; }
  /** Check if code is generated.
   * @return If code is generated.
   */
  bool getCode(void)
  { return Code; }
};

#endif
