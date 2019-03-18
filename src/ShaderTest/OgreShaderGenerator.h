/*
2 -----------------------------------------------------------------------------
3 This source file is part of OGRE
4 (Object-oriented Graphics Rendering Engine)
5 For the latest info, see http://www.ogre3d.org
6
7 Copyright (c) 2000-2014 Torus Knot Software Ltd
8 Permission is hereby granted, free of charge, to any person obtaining a copy
9 of this software and associated documentation files (the "Software"), to deal
10 in the Software without restriction, including without limitation the rights
11 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
12 copies of the Software, and to permit persons to whom the Software is
13 furnished to do so, subject to the following conditions:
14
15 The above copyright notice and this permission notice shall be included in
16 all copies or substantial portions of the Software.
17
18 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
19 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
20 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
21 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
22 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
23 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
24 THE SOFTWARE.
25 -----------------------------------------------------------------------------
26 */
#ifndef _ShaderGenerator_
#define _ShaderGenerator_

#include "OgreShaderPrerequisites.h"
#include "OgreSingleton.h"
#include "OgreFileSystemLayer.h"
#include "OgreRenderObjectListener.h"
#include "OgreSceneManager.h"
#include "OgreShaderRenderState.h"
#include "OgreScriptTranslator.h"
#include "OgreShaderScriptTranslator.h"


namespace Ogre {
	namespace RTShader {

		class _OgreRTSSExport ShaderGenerator : public Singleton<ShaderGenerator>, public RTShaderSystemAlloc
		{
			// Interface.
		public:

			static bool initialize();

			static void destroy();


			static ShaderGenerator& getSingleton();


			static ShaderGenerator* getSingletonPtr();

			void addSceneManager(SceneManager* sceneMgr);

			void removeSceneManager(SceneManager* sceneMgr);

			SceneManager* getActiveSceneManager();

			void setTargetLanguage(const String& shaderLanguage);

			const String& getTargetLanguage() const { return mShaderLanguage; }

			void setVertexShaderProfiles(const String& vertexShaderProfiles);

			const String& getVertexShaderProfiles() const { return mVertexShaderProfiles; }

			const StringVector& getVertexShaderProfilesList() const { return mVertexShaderProfilesList; }


			void setFragmentShaderProfiles(const String& fragmentShaderProfiles);

			const String& getFragmentShaderProfiles() const { return mFragmentShaderProfiles; }

			const StringVector& getFragmentShaderProfilesList() const { return mFragmentShaderProfilesList; }

			void setShaderCachePath(const String& cachePath);

			const String& getShaderCachePath() const { return mShaderCachePath; }

			void flushShaderCache();

			RenderState* getRenderState(const String& schemeName);


			typedef std::pair<RenderState*, bool> RenderStateCreateOrRetrieveResult;
			RenderStateCreateOrRetrieveResult createOrRetrieveRenderState(const String& schemeName);


			bool hasRenderState(const String& schemeName) const;


			RenderState* getRenderState(const String& schemeName, const String& materialName, unsigned short passIndex);

			RenderState* getRenderState(const String& schemeName, const String& materialName, const String& groupName, unsigned short passIndex);

			void addSubRenderStateFactory(SubRenderStateFactory* factory);

			size_t getNumSubRenderStateFactories() const;

			SubRenderStateFactory* getSubRenderStateFactory(size_t index);

			SubRenderStateFactory* getSubRenderStateFactory(const String& type);

			void removeSubRenderStateFactory(SubRenderStateFactory* factory);

			SubRenderState* createSubRenderState(const String& type);


			void destroySubRenderState(SubRenderState* subRenderState);


			bool hasShaderBasedTechnique(const String& materialName, const String& srcTechniqueSchemeName, const String& dstTechniqueSchemeName) const;

			bool hasShaderBasedTechnique(const String& materialName, const String& groupName, const String& srcTechniqueSchemeName, const String& dstTechniqueSchemeName) const;

			bool createShaderBasedTechnique(const String& materialName, const String& srcTechniqueSchemeName, const String& dstTechniqueSchemeName, bool overProgrammable = false);

			bool createShaderBasedTechnique(const String& materialName, const String& groupName, const String& srcTechniqueSchemeName, const String& dstTechniqueSchemeName, bool overProgrammable = false);


			bool removeShaderBasedTechnique(const String& materialName, const String& srcTechniqueSchemeName, const String& dstTechniqueSchemeName);

			bool removeShaderBasedTechnique(const String& materialName, const String& groupName, const String& srcTechniqueSchemeName, const String& dstTechniqueSchemeName);


			bool removeAllShaderBasedTechniques(const String& materialName, const String& groupName = ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);

			bool cloneShaderBasedTechniques(const String& srcMaterialName,
				const String& srcGroupName, const String& dstMaterialName, const String& dstGroupName);

			void removeAllShaderBasedTechniques();

			void createScheme(const String& schemeName);

			void invalidateScheme(const String& schemeName);

			bool validateScheme(const String& schemeName);

			void invalidateMaterial(const String& schemeName, const String& materialName, const String& groupName = ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);

			bool validateMaterial(const String& schemeName, const String& materialName, const String& groupName = ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);


			SGMaterialSerializerListener* getMaterialSerializerListener();


			size_t getVertexShaderCount() const;


			size_t getFragmentShaderCount() const;



			void setVertexShaderOutputsCompactPolicy(VSOutputCompactPolicy policy) { mVSOutputCompactPolicy = policy; }

			VSOutputCompactPolicy getVertexShaderOutputsCompactPolicy() const { return mVSOutputCompactPolicy; }


			void setCreateShaderOverProgrammablePass(bool value) { mCreateShaderOverProgrammablePass = value; }

			bool getCreateShaderOverProgrammablePass() const { return mCreateShaderOverProgrammablePass; }


			size_t getRTShaderSchemeCount() const;

			const String& getRTShaderScheme(size_t index) const;

			static String DEFAULT_SCHEME_NAME;

			// Protected types.
		protected:
			class SGPass;
			class SGTechnique;
			class SGMaterial;
			class SGScheme;

			typedef std::pair<String, String>                MatGroupPair;
			struct MatGroupPair_less
			{
				// ensure we arrange the list first by material name then by group name
				bool operator()(const MatGroupPair& p1, const MatGroupPair& p2) const
				{
					int cmpVal = strcmp(p1.first.c_str(), p2.first.c_str());
					return (cmpVal < 0) || ((cmpVal == 0) && (strcmp(p1.second.c_str(), p2.second.c_str()) < 0));
				}
			};

			typedef vector<SGPass*>::type                   SGPassList;
			typedef SGPassList::iterator                    SGPassIterator;
			typedef SGPassList::const_iterator              SGPassConstIterator;

			typedef vector<SGTechnique*>::type              SGTechniqueList;
			typedef SGTechniqueList::iterator               SGTechniqueIterator;
			typedef SGTechniqueList::const_iterator         SGTechniqueConstIterator;

			typedef map<SGTechnique*, SGTechnique*>::type   SGTechniqueMap;
			typedef SGTechniqueMap::iterator                SGTechniqueMapIterator;

			typedef map<MatGroupPair, SGMaterial*, MatGroupPair_less>::type SGMaterialMap;
			typedef SGMaterialMap::iterator                 SGMaterialIterator;
			typedef SGMaterialMap::const_iterator           SGMaterialConstIterator;

			typedef map<String, SGScheme*>::type            SGSchemeMap;
			typedef SGSchemeMap::iterator                   SGSchemeIterator;
			typedef SGSchemeMap::const_iterator             SGSchemeConstIterator;

			typedef map<String, ScriptTranslator*>::type    SGScriptTranslatorMap;
			typedef SGScriptTranslatorMap::iterator         SGScriptTranslatorIterator;
			typedef SGScriptTranslatorMap::const_iterator   SGScriptTranslatorConstIterator;



			class _OgreRTSSExport SGPass : public RTShaderSystemAlloc
			{
			public:
				SGPass(SGTechnique* parent, Pass* srcPass, Pass* dstPass);
				~SGPass();

				void buildTargetRenderState();

				void acquirePrograms();

				void releasePrograms();


				void notifyRenderSingleObject(Renderable* rend, const AutoParamDataSource* source, const LightList* pLightList, bool suppressRenderStateChanges);

				Pass* getSrcPass() { return mSrcPass; }

				Pass* getDstPass() { return mDstPass; }

				SubRenderState* getCustomFFPSubState(int subStateOrder);

				RenderState* getCustomRenderState() { return mCustomRenderState; }

				void setCustomRenderState(RenderState* customRenderState) { mCustomRenderState = customRenderState; }

				// Key name for associating with a Pass instance.
				static String UserKey;

			protected:
				SubRenderState * getCustomFFPSubState(int subStateOrder, const RenderState* renderState);

			protected:
				// Parent technique.
				SGTechnique * mParent;
				// Source pass.
				Pass* mSrcPass;
				// Destination pass.
				Pass* mDstPass;
				// Custom render state.
				RenderState* mCustomRenderState;
				// The compiled render state.       
				TargetRenderState* mTargetRenderState;
			};


			class _OgreRTSSExport SGTechnique : public RTShaderSystemAlloc
			{
			public:
				SGTechnique(SGMaterial* parent, Technique* srcTechnique, const String& dstTechniqueSchemeName);
				~SGTechnique();

				const SGMaterial* getParent() const { return mParent; }

				Technique* getSourceTechnique() { return mSrcTechnique; }

				Technique* getDestinationTechnique() { return mDstTechnique; }

				const String& getDestinationTechniqueSchemeName() const { return mDstTechniqueSchemeName; }

				void buildTargetRenderState();

				void acquirePrograms();

				void releasePrograms();

				void setBuildDestinationTechnique(bool buildTechnique) { mBuildDstTechnique = buildTechnique; }

				bool getBuildDestinationTechnique() const { return mBuildDstTechnique; }

				RenderState* getRenderState(unsigned short passIndex);
				bool hasRenderState(unsigned short passIndex);

				// Key name for associating with a Technique instance.
				static String UserKey;

			protected:

				void createSGPasses();

				void destroySGPasses();

			protected:
				// Parent material.     
				SGMaterial * mParent;
				// Source technique.
				Technique* mSrcTechnique;
				// Destination technique.
				Technique* mDstTechnique;
				// All passes entries.
				SGPassList mPassEntries;
				// The custom render states of all passes.
				RenderStateList mCustomRenderStates;
				// Flag that tells if destination technique should be build.        
				bool mBuildDstTechnique;
				// Scheme name of destination technique.
				String mDstTechniqueSchemeName;
			};


			class _OgreRTSSExport SGMaterial : public RTShaderSystemAlloc
			{

			public:
				SGMaterial(const String& materialName, const String& groupName) : mName(materialName), mGroup(groupName)
				{

				}

				const String& getMaterialName() const { return mName; }

				const String& getGroupName() const { return mGroup; }

				const SGTechniqueList& getTechniqueList() const { return mTechniqueEntries; }

				SGTechniqueList& getTechniqueList() { return mTechniqueEntries; }

			protected:
				// The material name.
				String mName;
				// The group name.
				String mGroup;
				// All passes entries.
				SGTechniqueList mTechniqueEntries;
			};


			class _OgreRTSSExport SGScheme : public RTShaderSystemAlloc
			{
			public:
				SGScheme(const String& schemeName);
				~SGScheme();


				bool empty() const { return mTechniqueEntries.empty(); }

				void invalidate();

				void validate();

				void invalidate(const String& materialName, const String& groupName = ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);

				bool validate(const String& materialName, const String& groupName = ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);

				void addTechniqueEntry(SGTechnique* techEntry);

				void removeTechniqueEntry(SGTechnique* techEntry);


				RenderState* getRenderState();

				RenderState* getRenderState(const String& materialName, const String& groupName, unsigned short passIndex);

			protected:
				void synchronizeWithLightSettings();

				void synchronizeWithFogSettings();


			protected:
				// Scheme name.
				String mName;
				// Technique entries.
				SGTechniqueList mTechniqueEntries;
				// Tells if this scheme is out of date.
				bool mOutOfDate;
				// The global render state of this scheme.
				RenderState* mRenderState;
				// Current fog mode.
				FogMode mFogMode;
			};


			// Protected types.
		protected:

			class _OgreRTSSExport SGRenderObjectListener : public RenderObjectListener, public RTShaderSystemAlloc
			{
			public:
				SGRenderObjectListener(ShaderGenerator* owner)
				{
					mOwner = owner;
				}

				virtual void notifyRenderSingleObject(Renderable* rend, const Pass* pass,
					const AutoParamDataSource* source,
					const LightList* pLightList, bool suppressRenderStateChanges)
				{
					mOwner->notifyRenderSingleObject(rend, pass, source, pLightList, suppressRenderStateChanges);
				}

			protected:
				ShaderGenerator * mOwner;
			};

			class _OgreRTSSExport SGSceneManagerListener : public SceneManager::Listener, public RTShaderSystemAlloc
			{
			public:
				SGSceneManagerListener(ShaderGenerator* owner)
				{
					mOwner = owner;
				}

				virtual void preFindVisibleObjects(SceneManager* source,
					SceneManager::IlluminationRenderStage irs, Viewport* v)
				{
					mOwner->preFindVisibleObjects(source, irs, v);
				}

				virtual void postFindVisibleObjects(SceneManager* source,
					SceneManager::IlluminationRenderStage irs, Viewport* v)
				{

				}

				virtual void shadowTexturesUpdated(size_t numberOfShadowTextures)
				{

				}

				virtual void shadowTextureCasterPreViewProj(Light* light,
					Camera* camera, size_t iteration)
				{

				}

				virtual void shadowTextureReceiverPreViewProj(Light* light,
					Frustum* frustum)
				{

				}

			protected:
				// The shader generator instance.
				ShaderGenerator * mOwner;
			};

			class _OgreRTSSExport SGScriptTranslatorManager : public ScriptTranslatorManager
			{
			public:
				SGScriptTranslatorManager(ShaderGenerator* owner)
				{
					mOwner = owner;
				}

				virtual size_t getNumTranslators() const
				{
					return mOwner->getNumTranslators();
				}

				virtual ScriptTranslator *getTranslator(const AbstractNodePtr& node)
				{
					return mOwner->getTranslator(node);
				}

			protected:
				// The shader generator instance.
				ShaderGenerator * mOwner;
			};

			//-----------------------------------------------------------------------------
			typedef map<String, SubRenderStateFactory*>::type       SubRenderStateFactoryMap;
			typedef SubRenderStateFactoryMap::iterator              SubRenderStateFactoryIterator;
			typedef SubRenderStateFactoryMap::const_iterator        SubRenderStateFactoryConstIterator;

			//-----------------------------------------------------------------------------
			typedef map<String, SceneManager*>::type                SceneManagerMap;
			typedef SceneManagerMap::iterator                       SceneManagerIterator;
			typedef SceneManagerMap::const_iterator                 SceneManagerConstIterator;

		protected:
			ShaderGenerator();

			~ShaderGenerator();

			bool _initialize();

			void _destroy();

			Technique* findSourceTechnique(const String& materialName, const String& groupName, const String& srcTechniqueSchemeName, bool allowProgrammable);

			bool isProgrammable(Technique* tech) const;

			void notifyRenderSingleObject(Renderable* rend, const Pass* pass, const AutoParamDataSource* source, const LightList* pLightList, bool suppressRenderStateChanges);

			void preFindVisibleObjects(SceneManager* source, SceneManager::IlluminationRenderStage irs, Viewport* v);

			void createSubRenderStateExFactories();

			void destroySubRenderStateExFactories();

			SubRenderState* createSubRenderState(ScriptCompiler* compiler, PropertyAbstractNode* prop, Pass* pass, SGScriptTranslator* translator);

			SubRenderState* createSubRenderState(ScriptCompiler* compiler, PropertyAbstractNode* prop, TextureUnitState* texState, SGScriptTranslator* translator);

			bool addCustomScriptTranslator(const String& key, ScriptTranslator* translator);

			bool removeCustomScriptTranslator(const String& key);

			size_t getNumTranslators() const;

			ScriptTranslator* getTranslator(const AbstractNodePtr& node);


			void serializePassAttributes(MaterialSerializer* ser, SGPass* passEntry);

			void serializeTextureUnitStateAttributes(MaterialSerializer* ser, SGPass* passEntry, const TextureUnitState* srcTextureUnit);

			SGMaterialIterator findMaterialEntryIt(const String& materialName, const String& groupName);
			SGMaterialConstIterator findMaterialEntryIt(const String& materialName, const String& groupName) const;


			typedef std::pair<SGScheme*, bool> SchemeCreateOrRetrieveResult;
			SchemeCreateOrRetrieveResult createOrRetrieveScheme(const String& schemeName);

			bool getIsFinalizing() const;
		protected:
			// Auto mutex.
			OGRE_AUTO_MUTEX;
			// The active scene manager.
			SceneManager* mActiveSceneMgr;
			// A map of all scene managers this generator is bound to.
			SceneManagerMap mSceneManagerMap;
			// Render object listener.
			SGRenderObjectListener* mRenderObjectListener;
			// Scene manager listener.
			SGSceneManagerListener* mSceneManagerListener;
			// Script translator manager.
			SGScriptTranslatorManager* mScriptTranslatorManager;
			// Custom material Serializer listener - allows exporting material that contains shader generated techniques.
			SGMaterialSerializerListener* mMaterialSerializerListener;
			// A map of the registered custom script translators.
			SGScriptTranslatorMap mScriptTranslatorsMap;
			// The core translator of the RT Shader System.
			SGScriptTranslator mCoreScriptTranslator;
			// The target shader language (currently only cg supported).
			String mShaderLanguage;
			// The target vertex shader profile. Will be used as argument for program compilation.
			String mVertexShaderProfiles;
			// List of target vertex shader profiles.
			StringVector mVertexShaderProfilesList;
			// The target fragment shader profile. Will be used as argument for program compilation.
			String mFragmentShaderProfiles;
			// List of target fragment shader profiles..
			StringVector mFragmentShaderProfilesList;
			// Path for caching the generated shaders.
			String mShaderCachePath;
			// Shader program manager.
			ProgramManager* mProgramManager;
			// Shader program writer manager.
			ProgramWriterManager* mProgramWriterManager;
			// File system layer manager.
			FileSystemLayer* mFSLayer;
			// Fixed Function Render state builder.
			FFPRenderStateBuilder* mFFPRenderStateBuilder;
			// Material entries map.
			SGMaterialMap mMaterialEntriesMap;
			// Scheme entries map.
			SGSchemeMap mSchemeEntriesMap;
			// All technique entries map.
			SGTechniqueMap mTechniqueEntriesMap;
			// Sub render state registered factories.
			SubRenderStateFactoryMap mSubRenderStateFactories;
			// Sub render state core extension factories.
			SubRenderStateFactoryMap mSubRenderStateExFactories;
			// True if active view port use a valid SGScheme.
			bool mActiveViewportValid;
			// Light count per light type.
			int mLightCount[3];
			// Vertex shader outputs compact policy.
			VSOutputCompactPolicy mVSOutputCompactPolicy;
			// Tells whether shaders are created for passes with shaders
			bool mCreateShaderOverProgrammablePass;
			// a flag to indicate finalizing
			bool mIsFinalizing;
		private:
			friend class SGPass;
			friend class FFPRenderStateBuilder;
			friend class SGScriptTranslatorManager;
			friend class SGScriptTranslator;
			friend class SGMaterialSerializerListener;

		};


	}

}

#endif