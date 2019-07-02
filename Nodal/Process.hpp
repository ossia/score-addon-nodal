#pragma once
#include <Process/GenericProcessFactory.hpp>
#include <Process/Process.hpp>

#include <Nodal/Metadata.hpp>

namespace Nodal
{
class Node final
    : public score::Entity<Node>
{
  W_OBJECT(Node)

public:
  Node(
      std::unique_ptr<Process::ProcessModel> proc,
      const Id<Node>& id,
      QObject* parent);

  template <typename Impl>
  Node(Impl& vis, QObject* parent)
    : score::Entity<Node>{vis, parent}
  {
    vis.writeTo(*this);
  }

  ~Node() override;

  QPointF position() const noexcept;
  QSizeF size() const noexcept;
  Process::ProcessModel& process() const noexcept;

  void setPosition(const QPointF& v);
  void setSize(const QSizeF& v);

  void positionChanged(QPointF p) W_SIGNAL(positionChanged, p);
  void sizeChanged(QSizeF p) W_SIGNAL(sizeChanged, p);
  void processChanged(Process::ProcessModel* p) W_SIGNAL(processChanged, p);

  PROPERTY(QPointF, position READ position WRITE setPosition NOTIFY positionChanged)
  PROPERTY(QSizeF, size READ size WRITE setSize NOTIFY sizeChanged)
private:
  std::unique_ptr<Process::ProcessModel> m_impl;
  QPointF m_position{};
  QSizeF m_size{};
};

class Model final : public Process::ProcessModel
{
  SCORE_SERIALIZE_FRIENDS
  PROCESS_METADATA_IMPL(Nodal::Model)
  W_OBJECT(Model)

public:
  Model(
      const TimeVal& duration, const Id<Process::ProcessModel>& id,
      QObject* parent);

  template <typename Impl>
  Model(Impl& vis, QObject* parent) : Process::ProcessModel{vis, parent}
  {
    vis.writeTo(*this);
  }

  ~Model() override;

  score::EntityMap<Node> nodes;
private:
  QString prettyName() const noexcept override;

  void setDurationAndScale(const TimeVal& newDuration) noexcept override;
  void setDurationAndGrow(const TimeVal& newDuration) noexcept override;
  void setDurationAndShrink(const TimeVal& newDuration) noexcept override;
};

using ProcessFactory = Process::ProcessFactory_T<Nodal::Model>;
}

W_REGISTER_ARGTYPE(Process::ProcessModel*)
