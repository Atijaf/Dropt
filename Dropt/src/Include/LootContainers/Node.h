#pragma once
#include "../Core/CoreLoot.h"
#include "../Core/Dispatch.h"

namespace Dropt {
	namespace impl {

		class AbstractNode
		{
		public:
			AbstractLootDispatcher* GetContainee() { return Containee; }
		protected:
			AbstractNode(AbstractLootDispatcher* _Containee) :
				Containee(_Containee) {};

			AbstractLootDispatcher* Containee;
		};

		template <typename LootType, Variance Variant>
		class BaseNode : public AbstractNode, public CoreLootContainer<LootType, Variant>
		{
		public:
			BaseNode(CoreLootContainer<LootType, Variant>& _NodeRef, AbstractLootDispatcher* _Containee) :
				CoreLootContainer<LootType, Variant>(NodeRef.GetBrother()),
				AbstractNode(_Containee),
				NodeRef(_NodeRef) 
			{
				NodeRef.GetBrother()->AddReference(this);
			};

			~BaseNode() {
				NodeRef.GetBrother()->RemoveReference(this);
			}

			bool GetLoot(std::list<LootType*>& OutLoot) override {
				return NodeRef.GetLoot(OutLoot);
			}

		protected:

			bool ShouldRemoveFromContainer() const override {
				return NodeRef.ShouldRemoveFromContainer();
			}


			bool IsFinalized() const override {
				return NodeRef.IsFinalized();
			}

			constexpr Obtainabilities GetObtainability() const override {
				return NodeRef.GetObtainability();
			}

			bool FinalizeLoot() override {
				return NodeRef.FinalizeLoot();
			};

			CoreLootContainer<LootType, Variant>& NodeRef;
		};

		template <typename LootType, Variance Variant>
		class Node : public BaseNode<LootType, Variant>
		{
		public:
			Node(CoreLootContainer<LootType, Variant>& _NodeRef, CoreLootContainer<LootType, Variant>* _Containee) :
				BaseNode<LootType,Variant>(_NodeRef,_Containee) {}
		};


		template <typename LootType>
		class Node<LootType, Variance::Chance> : public BaseNode<LootType, Variance::Chance>
		{
		public:
			Node(CoreLootContainer<LootType, Variance::Chance>& _NodeRef, AbstractLootDispatcher* _Containee) :
				BaseNode<LootType, Variance::Chance>(_NodeRef,_Containee) 
			{
				this->SetWeight(this->NodeRef.GetWeight());
			}
		};

		template <typename LootType>
		class Node<LootType, Variance::Interval> : public BaseNode<LootType, Variance::Interval>
		{
		public:
			Node(CoreLootContainer<LootType, Variance::Interval>& _NodeRef, AbstractLootDispatcher* _Containee) :
				BaseNode<LootType, Variance::Interval>(_NodeRef,_Containee) 
			{
				this->SetInterval(this->NodeRef.GetInterval());
			}
		};

		template <typename LootType>
		class Node<LootType, Variance::Constant> : public BaseNode<LootType, Variance::Constant>
		{
		public:
			Node(CoreLootContainer<LootType, Variance::Constant>& _NodeRef, AbstractLootDispatcher* _Containee) :
				BaseNode<LootType, Variance::Constant>(_NodeRef,_Containee) {};
		};

	}
}